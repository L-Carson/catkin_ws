# core_lib 预编译库说明

`lib/` 目录下的 `libbasics.a` 等预编译静态库**不纳入 git**（单文件最大约 110MB，超过 GitHub 限制）。

## Orin Nano (aarch64) 获取方式

编译 `comm` / `lidar_det_node` 前，需在本机准备好：

```
lib/arm64/libbasics.a
lib/arm64/libsaijie_basics.a
```

任选其一：

1. 从整车 `navZero` 工程拷贝 `core_lib/lib/arm64/`
2. 解压工作区内的 `src/comm.zip`（若存在）
3. 从内部制品库 / 同事处同步 comm 完整包

## 验证

```bash
ls -lh src/comm/core_lib/lib/arm64/libbasics.a
# 应存在且约 70MB
```
