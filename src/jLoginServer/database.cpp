#include "database.h"
#include "shared/database/cDBMgr.h"
#include "shared/cLogger.h"

#include <boost/any.hpp>
#include <map>
#include <cstring>

using namespace std;
using namespace boost;
using namespace jRylServer::common::shared;
using namespace jRylServer::common::shared::database;

namespace jRylServer {
namespace jLoginServer {

static Logger* LOG = Logger::GetInstance_ptr();

DB_STATUS authenticate_account(const char* user, const char* pass, RESULT& out) {
    char* qry = "select acc.accountid, acc.nation, ban.accountid as banid"
                "  from account acc"
                "          left join banned_account ban"
                "            on acc.accountid = ban.accountid"
                " where user = ?"
                "   and pass = ?";
    DBMgr::MyConnection* conn = DBMgr::GetInstance().MyGetConnection();

    MYSQL_STMT* stmt = my_prepare(qry, &conn->Conn);
    if(stmt == NULL) {
        return DS_ERROR;
    }

    uint32 accountid;
    int8 nation;
    uint32 banid;

    MYSQL_BIND param[2];
    memset(param, 0, sizeof(param));
    my_prepare_bind(&param[0], user, strlen(user));
    my_prepare_bind(&param[1], pass, strlen(pass));
    
    MYSQL_BIND result[3];
    memset(result, 0, sizeof(result));
    my_prepare_bind(&result[0], accountid);
    my_prepare_bind(&result[1], nation);
    my_prepare_bind(&result[2], banid);

    if(mysql_stmt_bind_param(stmt, param)) {
        LOG->ShowError("authenticate_account()[mysql_stmt_bind_param()]: [%d] %s.\n", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
        return DS_ERROR;
    }
    
    if(mysql_stmt_bind_result(stmt, result)) {
        LOG->ShowError("authenticate_account()[mysql_stmt_bind_result()]: [%d] %s.\n", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
        return DS_ERROR;
    }
    
    if (mysql_stmt_execute(stmt)) {
        LOG->ShowError("authenticate_account()[mysql_stmt_execute()]: [%d] %s.\n", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
        return DS_ERROR;
    }

    if(mysql_stmt_store_result(stmt)) {
        LOG->ShowError("authenticate_account()[mysql_stmt_store_result()]: [%d] %s.\n", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
        return DS_ERROR;
    }

    if(mysql_stmt_num_rows(stmt) != 1) {
        return DS_NO_DATA;
    }

    if(mysql_stmt_fetch(stmt)) {
        LOG->ShowError("authenticate_account()[mysql_stmt_store_result()]: [%d] %s.\n", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
        return DS_ERROR;
    }
    
    out["accountid"] = accountid;
    out["nation"] = nation;
    out["banid"] = result[2].is_null_value? 0: banid;
    
    DBMgr::GetInstance().MyFreeConnection(conn);

    return DS_SUCESS;
}

DB_STATUS load_characters_view(uint32 accountId, RESULT_SET& out) {
    char* qry = "select cf.charinfoid, gd.guildid, gd.name guildname, gd.nation guildnation, pt.partyid,"
                "       cf.slot, cf.name, cf.sex, cf.hair, cf.face, cf.class, cf.fame, cf.medal, cf.level"
                "  from charinfo cf"
                "           left join guild gd"
                "             on cf.guildid = gd.guildid"
                "           left join party pt"
                "             on cf.partyid = pt.partyid"
                "where cf.accountid = ?"
                "  and excluded = 0";
    DBMgr::MyConnection* conn = DBMgr::GetInstance().MyGetConnection();

    MYSQL_STMT* stmt = my_prepare(qry, &conn->Conn);
    if(stmt == NULL) {
        return DS_ERROR;
    }

    MYSQL_BIND param;
    memset(&param, 0, sizeof(param));
    my_prepare_bind(&param, accountId);
    
    uint32 charinfoid, guildid, partyid, fame, medal;
    char guildname[41], name[41];
    uint8 level;
    int8 guildnation, sex, hair, face;
    int16 class_;
    int32 slot;

    MYSQL_BIND result[14];
    memset(result, 0, sizeof(result));
    my_prepare_bind(&result[0], charinfoid);
    my_prepare_bind(&result[1], guildid);
    my_prepare_bind(&result[2], guildname, 41);
    my_prepare_bind(&result[3], guildnation);
    my_prepare_bind(&result[4], partyid);
    my_prepare_bind(&result[5], slot);
    my_prepare_bind(&result[6], name, 41);
    my_prepare_bind(&result[7], sex);
    my_prepare_bind(&result[8], hair);
    my_prepare_bind(&result[9], face);
    my_prepare_bind(&result[10], class_);
    my_prepare_bind(&result[11], fame);
    my_prepare_bind(&result[12], medal);
    my_prepare_bind(&result[13], level);

    if(mysql_stmt_bind_param(stmt, &param)) {
        LOG->ShowError("load_characters_view()[mysql_stmt_bind_param()]: [%d] %s.\n", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
        return DS_ERROR;
    }
    
    if(mysql_stmt_bind_result(stmt, result)) {
        LOG->ShowError("load_characters_view()[mysql_stmt_bind_result()]: [%d] %s.\n", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
        return DS_ERROR;
    }
    
    if (mysql_stmt_execute(stmt)) {
        LOG->ShowError("load_characters_view()[mysql_stmt_execute()]: [%d] %s.\n", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
        return DS_ERROR;
    }
  
    for(int i = 0;!mysql_stmt_fetch(stmt);i++) {
        out[i]["charinfoid"] = charinfoid;
        out[i]["guildid"] = guildid;
        out[i]["guildname"] = string(guildname);
        out[i]["guildnation"] = guildnation;
        out[i]["partyid"] = partyid;
        out[i]["slot"] = slot;
        out[i]["name"] = string(name);
        out[i]["sex"] = sex;
        out[i]["hair"] = hair;
        out[i]["face"] = face;
        out[i]["class"] = class_;
        out[i]["fame"] = fame;
        out[i]["medal"] = medal;
        out[i]["level"] = level;
    }
    return DS_SUCESS;
}

DB_STATUS load_equips_view(uint32 charinfoid, RESULT_SET& out) {
    char* qry = "select equippos, prototypeid"
                "  from charitem"
                " where charinfoid = ?"
                "   and equippos <> 0";
    DBMgr::MyConnection* conn = DBMgr::GetInstance().MyGetConnection();

    MYSQL_STMT* stmt = my_prepare(qry, &conn->Conn);
    if(stmt == NULL) {
        return DS_ERROR;
    }

    MYSQL_BIND param;
    memset(&param, 0, sizeof(param));
    my_prepare_bind(&param, charinfoid);
    
    int8 equippos;
    uint16 prototypeid;

    MYSQL_BIND result[2];
    memset(result, 0, sizeof(result));
    my_prepare_bind(&result[0], equippos);
    my_prepare_bind(&result[1], prototypeid);
    

    if(mysql_stmt_bind_param(stmt, &param)) {
        LOG->ShowError("load_equips_view()[mysql_stmt_bind_param()]: [%d] %s.\n", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
        return DS_ERROR;
    }
    
    if(mysql_stmt_bind_result(stmt, result)) {
        LOG->ShowError("load_equips_view()[mysql_stmt_bind_result()]: [%d] %s.\n", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
        return DS_ERROR;
    }
    
    if (mysql_stmt_execute(stmt)) {
        LOG->ShowError("load_equips_view()[mysql_stmt_execute()]: [%d] %s.\n", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
        return DS_ERROR;
    }
  
    for(int i = 0;!mysql_stmt_fetch(stmt);i++) {
        out[i]["equippos"] = equippos;
        out[i]["prototypeid"] = prototypeid;
    }
    return DS_SUCESS;
}

}
}