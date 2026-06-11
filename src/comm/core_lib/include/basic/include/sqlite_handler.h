/**
 * @file     sqlite_handler.h
 * @brief    对数据库整理封装
 * @details  对数据库整理封装
 * @author   华仔
 * @date     9/7/21
 */

#ifndef SQLITEHANDLER_H_
#define SQLITEHANDLER_H_

#include "sqlite3/sqlite3.h"
#include<pthread.h>
#include <iostream>
#include <boost/function.hpp>
#include <boost/bind.hpp>



class SqliteHandler
{
public:
    /**
     * 构造函数
     * @param fN 文件绝对路径，不要有中文，如果有中文需要调用者转成UTF-8后调用
     */
	SqliteHandler(const char*fN);
    /**
     * 析构函数
     */
	~SqliteHandler();
public:
	/**
	 * 除了查询指令其他操作数据库指令(创建表，删除表，表中增加记录，修改记录，删除记录)。数据库不存在会自动创建
	 * @param c_str_sql sql语句
	 * @return 0成功，其他失败
	 */
	int OperationCmd(const char*c_str_sql);
    /**
     * 操作数据库指令,一般是查询，统计指令
     * @tparam T 模板
     * @param c_str_sql sql语句
     * @param fp 回调函数指针
     * @param obj 类指针
     * @param type 类型，为fp的第二次参数
     * @return 0成功，其他失败
     */
    template<class T>
    int SelectCmd(const char*c_str_sql,void(T::*fp)(sqlite3_stmt *,int), T* obj,int type)
    {
        boost::function<void(sqlite3_stmt *,int)> fun;
        fun = boost::bind(fp,obj,_1,_2);
        return SqliteSelectCmd(c_str_sql,fun,type);
    }
	/**
	 * 删除数据库
	 * @return 0成功，其他失败
	 */
	int DeleteDb();
	/**
	 * 整理数据库->删除记录，数据库的文件大小不会变小，当频繁删除记录时或者删除表等就要隔一段时间整理数据库。注意整理数据库会花费一定的时间
	 * @return 0成功，其他失败
	 */
	int ArrangementDb();

private:
	/**
	 * 操作数据库指令
	 * @param c_str_sql sql语句
	 * @param fun 回调函数
	 * @param type type 类型，为fp的第二次参数
	 * @return 0成功，其他失败
	 */
    int SqliteSelectCmd(const char*c_str_sql,boost::function<void(sqlite3_stmt *,int)>& fun,int type);
private:
    std::string fileName;
    pthread_mutex_t sqlite_mutex;
//private:
//    //建议调用者实现此函数，写入到日志文件中
//    int log_out_string(bool writeFile,const char*fmt,...);
};


#endif /* SQLITEHANDLER_H_ */
