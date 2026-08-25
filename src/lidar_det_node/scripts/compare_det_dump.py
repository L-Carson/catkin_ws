#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Compare two lidar_det_node TransFusion det_dump TSV files.

Typical use (fusion vs single LiDAR on the same rosbag):
  python3 compare_det_dump.py \\
      --a ~/map_config/log/debug/lidar_det_dump/main_only.tsv \\
      --b ~/map_config/log/debug/lidar_det_dump/fused.tsv \\
      --name-a main_only --name-b fused \\
      --output ~/map_config/log/debug/lidar_det_dump/compare_report.txt

Dependencies: Python 3.6+ stdlib only.
"""

from __future__ import division, print_function

import argparse
import csv
import math
import os
import statistics
import sys
from collections import Counter, defaultdict
from dataclasses import dataclass, field
from typing import Dict, Iterable, List, Optional, Sequence, Tuple


# ---------------------------------------------------------------------------
# Data model
# ---------------------------------------------------------------------------

FLOAT_COLS = {
    "cloud_stamp_sec", "pts_in", "infer_ms", "raw_det", "kept",
    "score", "x", "y", "z", "w", "l", "h", "yaw", "vx", "vy",
}
INT_COLS = {"frame", "det_idx", "native_id", "published", "out_label"}


@dataclass
class DumpMeta:
    path: str
    created: str = ""
    backend: str = ""
    score_threshold: float = 0.0
    class_map_profile: str = ""
    input_topic: str = ""


@dataclass
class Detection:
    frame: int
    cloud_stamp_sec: float
    pts_in: int
    infer_ms: float
    raw_det: int
    kept: int
    det_idx: int
    native_id: int
    native_name: str
    score: float
    x: float
    y: float
    z: float
    w: float
    l: float
    h: float
    yaw: float
    vx: float
    vy: float
    filter_reason: str
    published: bool
    out_label: int
    out_label_name: str

    @property
    def range_m(self) -> float:
        return math.hypot(self.x, self.y)

    @property
    def label_key(self) -> str:
        if self.out_label_name and self.out_label_name not in ("", "UNKNOWN", "OTHER"):
            return self.out_label_name
        return self.native_name or "unknown"


@dataclass
class FrameSummary:
    frame: int
    cloud_stamp_sec: float
    pts_in: int
    infer_ms: float
    raw_det: int
    kept: int
    has_detection: bool


@dataclass
class DumpData:
    meta: DumpMeta
    frames: Dict[float, FrameSummary] = field(default_factory=dict)
    detections: List[Detection] = field(default_factory=list)
    published: List[Detection] = field(default_factory=list)
    raw_valid: List[Detection] = field(default_factory=list)


# ---------------------------------------------------------------------------
# Load
# ---------------------------------------------------------------------------

def _parse_meta_line(meta: DumpMeta, line: str) -> None:
    line = line.strip()
    if line.startswith("# created="):
        meta.created = line.split("=", 1)[1]
    elif line.startswith("# backend="):
        rest = line.split("=", 1)[1]
        parts = rest.split()
        meta.backend = parts[0]
        for p in parts[1:]:
            if p.startswith("score_threshold="):
                meta.score_threshold = float(p.split("=", 1)[1])
            elif p.startswith("class_map_profile="):
                meta.class_map_profile = p.split("=", 1)[1]
            elif p.startswith("input_topic="):
                meta.input_topic = p.split("=", 1)[1]


def _to_bool(v: str) -> bool:
    return (v or "").strip() in ("1", "true", "True", "yes")


def _to_float(v: str, default: float = 0.0) -> float:
    v = (v or "").strip()
    if not v:
        return default
    try:
        return float(v)
    except ValueError:
        return default


def _to_int(v: str, default: int = 0) -> int:
    v = (v or "").strip()
    if not v:
        return default
    try:
        return int(float(v))
    except ValueError:
        return default


def load_dump(path: str) -> DumpData:
    path = os.path.expanduser(path)
    meta = DumpMeta(path=path)
    rows: List[dict] = []

    with open(path, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            if not line.strip():
                continue
            if line.startswith("#"):
                _parse_meta_line(meta, line)
                continue
            # header row handled by DictReader below — read file again for body
            break

    with open(path, "r", encoding="utf-8", errors="replace") as f:
        lines = [ln for ln in f if ln.strip() and not ln.startswith("#")]
    if not lines:
        raise ValueError("empty dump (no data rows): {}".format(path))

    reader = csv.DictReader(lines, delimiter="\t")
    for row in reader:
        rows.append(row)

    data = DumpData(meta=meta)
    seen_frames: Dict[float, FrameSummary] = {}

    for row in rows:
        stamp = _to_float(row.get("cloud_stamp_sec", "0"))
        frame = _to_int(row.get("frame", "0"))
        pts_in = _to_int(row.get("pts_in", "0"))
        infer_ms = _to_float(row.get("infer_ms", "0"))
        raw_det = _to_int(row.get("raw_det", "0"))
        kept = _to_int(row.get("kept", "0"))
        det_idx = _to_int(row.get("det_idx", "-1"), -1)
        reason = (row.get("filter_reason") or "").strip()

        if stamp not in seen_frames:
            seen_frames[stamp] = FrameSummary(
                frame=frame,
                cloud_stamp_sec=stamp,
                pts_in=pts_in,
                infer_ms=infer_ms,
                raw_det=raw_det,
                kept=kept,
                has_detection=(raw_det > 0),
            )

        if det_idx < 0 or reason == "no_detection":
            continue

        det = Detection(
            frame=frame,
            cloud_stamp_sec=stamp,
            pts_in=pts_in,
            infer_ms=infer_ms,
            raw_det=raw_det,
            kept=kept,
            det_idx=det_idx,
            native_id=_to_int(row.get("native_id", "-1"), -1),
            native_name=(row.get("native_name") or "").strip(),
            score=_to_float(row.get("score", "0")),
            x=_to_float(row.get("x", "0")),
            y=_to_float(row.get("y", "0")),
            z=_to_float(row.get("z", "0")),
            w=_to_float(row.get("w", "0")),
            l=_to_float(row.get("l", "0")),
            h=_to_float(row.get("h", "0")),
            yaw=_to_float(row.get("yaw", "0")),
            vx=_to_float(row.get("vx", "0")),
            vy=_to_float(row.get("vy", "0")),
            filter_reason=reason,
            published=_to_bool(row.get("published", "0")),
            out_label=_to_int(row.get("out_label", "0")),
            out_label_name=(row.get("out_label_name") or "").strip(),
        )
        data.detections.append(det)
        data.raw_valid.append(det)
        if det.published:
            data.published.append(det)

    data.frames = seen_frames
    return data


# ---------------------------------------------------------------------------
# Stats helpers
# ---------------------------------------------------------------------------

def _pct(n: float, total: float) -> float:
    if total <= 0:
        return 0.0
    return 100.0 * n / total


def _safe_mean(vals: Sequence[float]) -> Optional[float]:
    return statistics.mean(vals) if vals else None


def _safe_median(vals: Sequence[float]) -> Optional[float]:
    return statistics.median(vals) if vals else None


def _percentile(vals: Sequence[float], p: float) -> Optional[float]:
    if not vals:
        return None
    s = sorted(vals)
    k = (len(s) - 1) * p / 100.0
    f = math.floor(k)
    c = math.ceil(k)
    if f == c:
        return s[int(k)]
    return s[f] * (c - k) + s[c] * (k - f)


def _delta_str(a: Optional[float], b: Optional[float], unit: str = "",
               higher_is_better: Optional[bool] = None) -> str:
    if a is None or b is None:
        return "n/a"
    d = b - a
    sign = "+" if d >= 0 else ""
    hint = ""
    if higher_is_better is not None and abs(d) > 1e-9:
        improved = (d > 0) if higher_is_better else (d < 0)
        hint = " [better]" if improved else " [worse]"
    return "{}{:.4g}{}{}".format(sign, d, unit, hint)


def _fmt_opt(v: Optional[float], digits: int = 3) -> str:
    if v is None:
        return "n/a"
    return "{:.{}f}".format(v, digits)


def count_by(items: Iterable[Detection], key_fn) -> Counter:
    c: Counter = Counter()
    for it in items:
        c[key_fn(it)] += 1
    return c


def score_stats(dets: Sequence[Detection]) -> dict:
    scores = [d.score for d in dets]
    if not scores:
        return {}
    return {
        "count": len(scores),
        "mean": statistics.mean(scores),
        "median": statistics.median(scores),
        "min": min(scores),
        "max": max(scores),
        "p25": _percentile(scores, 25),
        "p75": _percentile(scores, 75),
        "p90": _percentile(scores, 90),
    }


def range_bucket(r: float) -> str:
    edges = [0, 20, 40, 60, 80, 100]
    labels = ["0-20m", "20-40m", "40-60m", "60-80m", "80-100m", "100m+"]
    for i, edge in enumerate(edges):
        if r < edge:
            return labels[i - 1] if i > 0 else labels[0]
    return labels[-1]


def sector_xy(x: float, y: float) -> str:
    if x >= 0 and abs(x) >= abs(y):
        return "front"
    if x < 0 and abs(x) >= abs(y):
        return "rear"
    if y >= 0:
        return "left"
    return "right"


def frame_empty_count(frames: Dict[float, FrameSummary]) -> int:
    return sum(1 for f in frames.values() if f.raw_det == 0)


def frame_pts_stats(frames: Dict[float, FrameSummary]) -> dict:
    vals = [f.pts_in for f in frames.values()]
    if not vals:
        return {}
    return {
        "mean": statistics.mean(vals),
        "median": statistics.median(vals),
        "min": min(vals),
        "max": max(vals),
    }


def frame_infer_stats(frames: Dict[float, FrameSummary]) -> dict:
    vals = [f.infer_ms for f in frames.values() if f.infer_ms > 0]
    if not vals:
        return {}
    return {
        "mean": statistics.mean(vals),
        "median": statistics.median(vals),
        "p95": _percentile(vals, 95),
    }


# ---------------------------------------------------------------------------
# Box matching (per timestamp)
# ---------------------------------------------------------------------------

def _match_boxes(
    list_a: Sequence[Detection],
    list_b: Sequence[Detection],
    match_dist_m: float,
    require_same_label: bool,
) -> Tuple[List[Tuple[Detection, Detection]], List[Detection], List[Detection]]:
    """Greedy nearest-neighbor matching in BEV."""
    pairs: List[Tuple[Detection, Detection, float]] = []
    for i, da in enumerate(list_a):
        for j, db in enumerate(list_b):
            if require_same_label and da.label_key != db.label_key:
                continue
            dist = math.hypot(da.x - db.x, da.y - db.y)
            if dist <= match_dist_m:
                pairs.append((da, db, dist))

    pairs.sort(key=lambda t: t[2])
    used_a = set()
    used_b = set()
    matched: List[Tuple[Detection, Detection]] = []
    for da, db, _ in pairs:
        ia = id(da)
        ib = id(db)
        if ia in used_a or ib in used_b:
            continue
        used_a.add(ia)
        used_b.add(ib)
        matched.append((da, db))

    only_a = [d for d in list_a if id(d) not in used_a]
    only_b = [d for d in list_b if id(d) not in used_b]
    return matched, only_a, only_b


# ---------------------------------------------------------------------------
# Report
# ---------------------------------------------------------------------------

class Report:
    def __init__(self, out_file=None):
        self.lines: List[str] = []
        self.out_file = out_file

    def w(self, text: str = "") -> None:
        self.lines.append(text)
        print(text)

    def h1(self, title: str) -> None:
        self.w("")
        self.w("=" * 72)
        self.w(title)
        self.w("=" * 72)

    def h2(self, title: str) -> None:
        self.w("")
        self.w("-" * 56)
        self.w(title)
        self.w("-" * 56)

    def table2(self, label: str, val_a, val_b, delta: str = "") -> None:
        if delta:
            self.w("  {:40s}  A={!s:>14s}  B={!s:>14s}  Δ={}".format(
                label, val_a, val_b, delta))
        else:
            self.w("  {:40s}  A={!s:>14s}  B={!s:>14s}".format(
                label, val_a, val_b))

    def save(self) -> None:
        if self.out_file:
            path = os.path.expanduser(self.out_file)
            os.makedirs(os.path.dirname(path) or ".", exist_ok=True)
            with open(path, "w", encoding="utf-8") as f:
                f.write("\n".join(self.lines) + "\n")
            print("\n[report saved] {}".format(path), file=sys.stderr)


def compare_dumps(
    data_a: DumpData,
    data_b: DumpData,
    name_a: str,
    name_b: str,
    match_dist_m: float,
    stamp_tol: float,
    top_n: int,
) -> Report:
    rep = Report()

    rep.h1("lidar_det_node det_dump comparison")
    rep.w("  A ({}) : {}".format(name_a, data_a.meta.path))
    rep.w("  B ({}) : {}".format(name_b, data_b.meta.path))

    rep.h2("1. Run metadata (from dump header)")
    for tag, meta in ((name_a, data_a.meta), (name_b, data_b.meta)):
        rep.w("  [{}]".format(tag))
        rep.w("    created           : {}".format(meta.created or "n/a"))
        rep.w("    backend           : {}".format(meta.backend or "n/a"))
        rep.w("    score_threshold   : {}".format(meta.score_threshold))
        rep.w("    class_map_profile : {}".format(meta.class_map_profile or "n/a"))
        rep.w("    input_topic       : {}".format(meta.input_topic or "n/a"))

    fa, fb = data_a.frames, data_b.frames
    stamps_a = set(fa.keys())
    stamps_b = set(fb.keys())
    common = stamps_a & stamps_b
    only_a = stamps_a - stamps_b
    only_b = stamps_b - stamps_a

    rep.h2("2. Frame alignment (by cloud_stamp_sec)")
    rep.table2("total frames", len(stamps_a), len(stamps_b),
               _delta_str(len(stamps_a), len(stamps_b), "", True))
    rep.table2("matched timestamps", len(common), len(common), "")
    rep.table2("timestamps only in A", len(only_a), "-", "")
    rep.table2("timestamps only in B", "-", len(only_b), "")

    if stamp_tol > 0 and (only_a or only_b):
        rep.w("  (stamp_tol={} not used for fuzzy match; exact stamp only)".format(stamp_tol))

    empty_a = frame_empty_count(fa)
    empty_b = frame_empty_count(fb)
    rep.h2("3. Frame-level detection rate")
    rep.table2("frames with raw_det=0", empty_a, empty_b,
               _delta_str(empty_a, empty_b, "", False))
    rep.table2("empty-frame ratio",
               "{:.1f}%".format(_pct(empty_a, len(fa))),
               "{:.1f}%".format(_pct(empty_b, len(fb))),
               _delta_str(_pct(empty_a, len(fa)), _pct(empty_b, len(fb)), "pp", False))

    pts_a = frame_pts_stats(fa)
    pts_b = frame_pts_stats(fb)
    rep.h2("4. Input point cloud size (pts_in) — fusion usually increases this")
    if pts_a and pts_b:
        rep.table2("mean pts_in", _fmt_opt(pts_a["mean"], 0), _fmt_opt(pts_b["mean"], 0),
                   _delta_str(pts_a["mean"], pts_b["mean"], "", True))
        rep.table2("median pts_in", _fmt_opt(pts_a["median"], 0), _fmt_opt(pts_b["median"], 0),
                   _delta_str(pts_a["median"], pts_b["median"], "", True))
        rep.table2("min / max pts_in",
                   "{} / {}".format(int(pts_a["min"]), int(pts_a["max"])),
                   "{} / {}".format(int(pts_b["min"]), int(pts_b["max"])), "")

    inf_a = frame_infer_stats(fa)
    inf_b = frame_infer_stats(fb)
    rep.h2("5. Inference latency (infer_ms per frame)")
    if inf_a and inf_b:
        rep.table2("mean infer_ms", _fmt_opt(inf_a["mean"]), _fmt_opt(inf_b["mean"]),
                   _delta_str(inf_a["mean"], inf_b["mean"], " ms", False))
        rep.table2("median infer_ms", _fmt_opt(inf_a["median"]), _fmt_opt(inf_b["median"]),
                   _delta_str(inf_a["median"], inf_b["median"], " ms", False))
        rep.table2("p95 infer_ms", _fmt_opt(inf_a["p95"]), _fmt_opt(inf_b["p95"]),
                   _delta_str(inf_a["p95"], inf_b["p95"], " ms", False))

    rep.h2("6. Detection counts (all frames)")
    rep.table2("raw detections (all boxes)",
               len(data_a.raw_valid), len(data_b.raw_valid),
               _delta_str(len(data_a.raw_valid), len(data_b.raw_valid), "", True))
    rep.table2("published (kept output)",
               len(data_a.published), len(data_b.published),
               _delta_str(len(data_a.published), len(data_b.published), "", True))
    pub_rate_a = _pct(len(data_a.published), len(data_a.raw_valid))
    pub_rate_b = _pct(len(data_b.published), len(data_b.raw_valid))
    rep.table2("publish rate (pub/raw)",
               "{:.1f}%".format(pub_rate_a), "{:.1f}%".format(pub_rate_b),
               _delta_str(pub_rate_a, pub_rate_b, "pp", True))

    rep.h2("7. Filter pipeline (why boxes were dropped)")
    for tag, dets in ((name_a, data_a.raw_valid), (name_b, data_b.raw_valid)):
        c = Counter(d.filter_reason for d in dets)
        rep.w("  [{}] total raw={}".format(tag, len(dets)))
        for reason, n in c.most_common():
            rep.w("    {:16s} {:6d} ({:.1f}%)".format(
                reason, n, _pct(n, len(dets))))

    rep.w("")
    rep.w("  Delta (B - A) on filter counts:")
    ca = Counter(d.filter_reason for d in data_a.raw_valid)
    cb = Counter(d.filter_reason for d in data_b.raw_valid)
    all_reasons = sorted(set(ca) | set(cb))
    for r in all_reasons:
        rep.w("    {:16s}  A={:5d}  B={:5d}  Δ={:+d}".format(r, ca[r], cb[r], cb[r] - ca[r]))

    rep.h2("8. Score statistics — published boxes")
    for tag, dets in ((name_a, data_a.published), (name_b, data_b.published)):
        ss = score_stats(dets)
        rep.w("  [{}] n={}".format(tag, ss.get("count", 0)))
        if ss:
            rep.w("    mean={:.4f}  median={:.4f}  min={:.4f}  max={:.4f}".format(
                ss["mean"], ss["median"], ss["min"], ss["max"]))
            rep.w("    p25={:.4f}  p75={:.4f}  p90={:.4f}".format(
                ss["p25"], ss["p75"], ss["p90"]))

    ss_a = score_stats(data_a.published)
    ss_b = score_stats(data_b.published)
    if ss_a and ss_b:
        rep.w("  Δ mean score (B-A): {}".format(
            _delta_str(ss_a["mean"], ss_b["mean"], "", True)))

    rep.h2("9. Published count by output label (CAR / CYCLIST / PEDESTRIAN)")
    pa = count_by(data_a.published, lambda d: d.out_label_name or "UNKNOWN")
    pb = count_by(data_b.published, lambda d: d.out_label_name or "UNKNOWN")
    labels = sorted(set(pa) | set(pb), key=lambda x: (-max(pa[x], pb[x]), x))
    rep.w("  {:20s}  {:>8s}  {:>8s}  {:>8s}".format("label", name_a, name_b, "Δ"))
    for lbl in labels:
        rep.w("  {:20s}  {:8d}  {:8d}  {:+8d}".format(lbl, pa[lbl], pb[lbl], pb[lbl] - pa[lbl]))

    rep.h2("10. Raw count by native class (model output)")
    na = count_by(data_a.raw_valid, lambda d: d.native_name or str(d.native_id))
    nb = count_by(data_b.raw_valid, lambda d: d.native_name or str(d.native_id))
    natives = sorted(set(na) | set(nb), key=lambda x: (-max(na[x], nb[x]), x))
    rep.w("  {:20s}  {:>8s}  {:>8s}  {:>8s}".format("native", name_a, name_b, "Δ"))
    for lbl in natives:
        rep.w("  {:20s}  {:8d}  {:8d}  {:+8d}".format(lbl, na[lbl], nb[lbl], nb[lbl] - na[lbl]))

    rep.h2("11. Spatial coverage — published boxes by range (from ego, BEV)")
    order = ["0-20m", "20-40m", "40-60m", "60-80m", "80-100m", "100m+"]
    ra = count_by(data_a.published, lambda d: range_bucket(d.range_m))
    rb = count_by(data_b.published, lambda d: range_bucket(d.range_m))
    rep.w("  {:12s}  {:>8s}  {:>8s}  {:>8s}  comment".format("range", name_a, name_b, "Δ"))
    for bucket in order:
        da, db = ra[bucket], rb[bucket]
        note = ""
        if db > da and bucket in ("40-60m", "60-80m", "80-100m", "100m+"):
            note = "<- fusion often helps here"
        rep.w("  {:12s}  {:8d}  {:8d}  {:+8d}  {}".format(
            bucket, da, db, db - da, note))

    rep.h2("12. Spatial coverage — published boxes by sector (base_link)")
    sectors = ["front", "rear", "left", "right"]
    sa = count_by(data_a.published, lambda d: sector_xy(d.x, d.y))
    sb = count_by(data_b.published, lambda d: sector_xy(d.x, d.y))
    rep.w("  {:12s}  {:>8s}  {:>8s}  {:>8s}  comment".format("sector", name_a, name_b, "Δ"))
    for sec in sectors:
        da, db = sa[sec], sb[sec]
        note = ""
        if db > da and sec in ("rear", "left", "right"):
            note = "<- blind-spot / side coverage"
        rep.w("  {:12s}  {:8d}  {:8d}  {:+8d}  {}".format(
            sec, da, db, db - da, note))

    rep.h2("13. Per-frame comparison on matched timestamps")
    delta_pub: List[int] = []
    delta_raw: List[int] = []
    delta_pts: List[int] = []
    gain_frames = 0
    loss_frames = 0
    same_frames = 0

    for stamp in sorted(common):
        fa_f = fa[stamp]
        fb_f = fb[stamp]
        dp = fb_f.kept - fa_f.kept
        dr = fb_f.raw_det - fa_f.raw_det
        dpts = fb_f.pts_in - fa_f.pts_in
        delta_pub.append(dp)
        delta_raw.append(dr)
        delta_pts.append(dpts)
        if dp > 0:
            gain_frames += 1
        elif dp < 0:
            loss_frames += 1
        else:
            same_frames += 1

    if common:
        rep.table2("frames B kept > A kept", gain_frames, gain_frames, "")
        rep.table2("frames B kept < A kept", loss_frames, loss_frames, "")
        rep.table2("frames same kept", same_frames, same_frames, "")
        rep.table2("mean Δ kept per frame", _fmt_opt(_safe_mean(delta_pub), 2),
                   _fmt_opt(_safe_mean(delta_pub), 2), "")
        rep.table2("mean Δ raw_det per frame", _fmt_opt(_safe_mean(delta_raw), 2),
                   _fmt_opt(_safe_mean(delta_raw), 2), "")
        rep.table2("mean Δ pts_in per frame", _fmt_opt(_safe_mean(delta_pts), 0),
                   _fmt_opt(_safe_mean(delta_pts), 0), "")

        # frames with largest published gain
        rep.w("")
        rep.w("  Top {} frames with most published gain (B - A):".format(top_n))
        frame_gains = []
        for stamp in common:
            d = fb[stamp].kept - fa[stamp].kept
            if d > 0:
                frame_gains.append((d, stamp, fa[stamp], fb[stamp]))
        frame_gains.sort(reverse=True)
        for d, stamp, f_a, f_b in frame_gains[:top_n]:
            rep.w("    stamp={:.3f}  Δkept={:+d}  A: raw={} kept={} pts={}  "
                  "B: raw={} kept={} pts={}".format(
                      stamp, d, f_a.raw_det, f_a.kept, f_a.pts_in,
                      f_b.raw_det, f_b.kept, f_b.pts_in))

    rep.h2("14. Box-level matching on common timestamps (published only)")
    rep.w("  match_dist={:.1f}m, same out_label required".format(match_dist_m))

    total_matched = 0
    total_only_a = 0
    total_only_b = 0
    score_deltas: List[float] = []
    pos_deltas: List[float] = []
    only_b_by_range: Counter = Counter()
    only_b_by_sector: Counter = Counter()
    only_a_by_range: Counter = Counter()

    pub_by_stamp_a: Dict[float, List[Detection]] = defaultdict(list)
    pub_by_stamp_b: Dict[float, List[Detection]] = defaultdict(list)
    for d in data_a.published:
        pub_by_stamp_a[d.cloud_stamp_sec].append(d)
    for d in data_b.published:
        pub_by_stamp_b[d.cloud_stamp_sec].append(d)

    for stamp in sorted(common):
        ma, oa, ob = _match_boxes(
            pub_by_stamp_a[stamp], pub_by_stamp_b[stamp],
            match_dist_m, require_same_label=True)
        total_matched += len(ma)
        total_only_a += len(oa)
        total_only_b += len(ob)
        for da, db in ma:
            score_deltas.append(db.score - da.score)
            pos_deltas.append(math.hypot(db.x - da.x, db.y - da.y))
        for d in ob:
            only_b_by_range[range_bucket(d.range_m)] += 1
            only_b_by_sector[sector_xy(d.x, d.y)] += 1
        for d in oa:
            only_a_by_range[range_bucket(d.range_m)] += 1

    rep.table2("matched box pairs", total_matched, total_matched, "")
    rep.table2("boxes only in A (lost when switching to B)", total_only_a, total_only_a, "")
    rep.table2("boxes only in B (new vs A)", total_only_b, total_only_b, "")

    if score_deltas:
        rep.w("  Matched pairs score delta (B - A):")
        rep.w("    mean={:.4f}  median={:.4f}  improved(B>A)={}/{} ({:.1f}%)".format(
            statistics.mean(score_deltas),
            statistics.median(score_deltas),
            sum(1 for x in score_deltas if x > 0),
            len(score_deltas),
            _pct(sum(1 for x in score_deltas if x > 0), len(score_deltas)),
        ))
    if pos_deltas:
        rep.w("  Matched pairs BEV position shift |B-A| (m):")
        rep.w("    mean={:.3f}  median={:.3f}  p95={:.3f}".format(
            statistics.mean(pos_deltas),
            statistics.median(pos_deltas),
            _percentile(pos_deltas, 95) or 0,
        ))

    if total_only_b:
        rep.w("")
        rep.w("  New boxes in B by range (fusion gains):")
        for bucket in order:
            if only_b_by_range[bucket]:
                rep.w("    {:12s}  {:5d}".format(bucket, only_b_by_range[bucket]))
        rep.w("  New boxes in B by sector:")
        for sec in sectors:
            if only_b_by_sector[sec]:
                rep.w("    {:12s}  {:5d}".format(sec, only_b_by_sector[sec]))

    if total_only_a:
        rep.w("")
        rep.w("  Lost boxes (in A not matched in B) by range:")
        for bucket in order:
            if only_a_by_range[bucket]:
                rep.w("    {:12s}  {:5d}".format(bucket, only_a_by_range[bucket]))

    rep.h2("15. Fusion-oriented summary (auto interpretation)")
    bullets: List[str] = []

    if pts_a and pts_b and pts_b["mean"] > pts_a["mean"] * 1.05:
        bullets.append(
            "Input points increased {:.0f} -> {:.0f} mean pts_in (+{:.1f}%), "
            "consistent with multi-LiDAR fusion feeding denser clouds.".format(
                pts_a["mean"], pts_b["mean"],
                _pct(pts_b["mean"] - pts_a["mean"], pts_a["mean"])))

    if len(data_b.published) > len(data_a.published):
        bullets.append(
            "Published detections: {} -> {} (+{}, +{:.1f}%).".format(
                len(data_a.published), len(data_b.published),
                len(data_b.published) - len(data_a.published),
                _pct(len(data_b.published) - len(data_a.published), max(1, len(data_a.published)))))

    if empty_b < empty_a:
        bullets.append(
            "Empty frames reduced {} -> {} ({} fewer frames with no raw detection).".format(
                empty_a, empty_b, empty_a - empty_b))

    far_a = sum(ra[b] for b in ("40-60m", "60-80m", "80-100m", "100m+"))
    far_b = sum(rb[b] for b in ("40-60m", "60-80m", "80-100m", "100m+"))
    if far_b > far_a:
        bullets.append(
            "Long-range published boxes (>=40m): {} -> {} (+{}). "
            "Fusion may improve far-object recall.".format(far_a, far_b, far_b - far_a))

    blind_a = sa["rear"] + sa["left"] + sa["right"]
    blind_b = sb["rear"] + sb["left"] + sb["right"]
    if blind_b > blind_a:
        bullets.append(
            "Non-front sector boxes (rear+left+right): {} -> {} (+{}). "
            "Blind LiDAR fusion may improve side/rear coverage.".format(
                blind_a, blind_b, blind_b - blind_a))

    if total_only_b > total_only_a:
        bullets.append(
            "Box matching: {} new published boxes in B vs {} lost from A "
            "(net +{}).".format(total_only_b, total_only_a, total_only_b - total_only_a))

    if ss_a and ss_b and ss_b["mean"] > ss_a["mean"] + 0.01:
        bullets.append(
            "Mean published score improved {:.4f} -> {:.4f} (+{:.4f}).".format(
                ss_a["mean"], ss_b["mean"], ss_b["mean"] - ss_a["mean"]))

    low_a = ca.get("low_score", 0)
    low_b = cb.get("low_score", 0)
    if low_b < low_a:
        bullets.append(
            "Fewer low_score rejections: {} -> {} ({}). "
            "Some weak detections may have gained confidence after fusion.".format(
                low_a, low_b, low_a - low_b))

    if not bullets:
        bullets.append("No strong fusion advantage detected on aggregate metrics; "
                       "inspect per-class and spatial sections above.")

    for i, b in enumerate(bullets, 1):
        rep.w("  {}. {}".format(i, b))

    rep.h2("16. Usage notes")
    rep.w("  - Compare the same rosbag with only input_topic / fusion config changed.")
    rep.w("  - A=baseline (e.g. main LiDAR only), B=candidate (e.g. fused cloud).")
    rep.w("  - Large timestamp mismatch => different bag segments or clock issues.")
    rep.w("  - Tune --match-dist for box matching (default 2.0 m).")

    return rep


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Compare two lidar_det_node det_dump TSV files (fusion A/B analysis).",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Example:
  python3 compare_det_dump.py \\
    --a ~/dump/main_only.tsv --b ~/dump/fused.tsv \\
    --name-a main_only --name-b fused \\
    --output ~/dump/compare_report.txt
""")
    parser.add_argument("--a", required=True, help="Baseline dump TSV (e.g. single LiDAR)")
    parser.add_argument("--b", required=True, help="Candidate dump TSV (e.g. fused)")
    parser.add_argument("--name-a", default="A", help="Display name for --a")
    parser.add_argument("--name-b", default="B", help="Display name for --b")
    parser.add_argument("--output", "-o", default="",
                        help="Save report to this text file")
    parser.add_argument("--match-dist", type=float, default=2.0,
                        help="BEV center distance (m) for box matching (default 2.0)")
    parser.add_argument("--stamp-tol", type=float, default=0.0,
                        help="Reserved for fuzzy stamp match (default exact)")
    parser.add_argument("--top-n", type=int, default=10,
                        help="Show top N frames with largest published gain")
    args = parser.parse_args()

    try:
        data_a = load_dump(args.a)
        data_b = load_dump(args.b)
    except (IOError, ValueError) as e:
        print("ERROR: {}".format(e), file=sys.stderr)
        return 1

    rep = compare_dumps(
        data_a, data_b,
        args.name_a, args.name_b,
        args.match_dist, args.stamp_tol, args.top_n,
    )
    rep.out_file = args.output
    rep.save()
    return 0


if __name__ == "__main__":
    sys.exit(main())
