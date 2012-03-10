#ifndef STD_DEFS_H
#define	STD_DEFS_H

//Packet defs
#define PACKET_HEADER_SIZE 12
#define PACKET_TYPE_NORMAL 0x8000
#define PACKET_TYPE_COMPRESSED 0x4000
#define PACKET_START_BIT 0xFF

//Buffer defs
#define BUFFER_DEFALT_MAX_LENGTH 4096

//General defs
#define CHAR_SLOT_LENGTH 5
#define FILE_CONF_GLOBAL_INI "config/global.ini"
#define FILE_CONF_LOGINSERVER_INI "config/jLoginServer.ini"
#define FILE_CONF_AUTHSERVER_INI "config/servers/{0}/jAuthServer.ini"

//AuthServer defs
#define AUTHSERVER_SLOT_LENGTH 10
#define MAX_AUTHSERVER_LOGIN_ATTEMPTS 3

#endif