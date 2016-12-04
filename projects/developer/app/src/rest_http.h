/************** WIFI MODULE CMD *******************/
#define AT_CMD "AT\r\n"
#define AT_MODE "AT+CWMODE=1\r\n"
#define AT_LAP "AT+CWLAP\r\n"
#define AT_QAP "AT+CWQAP\r\n"
#define AT_JAP "AT+CWJAP=\"NTD\",\"\"\r\n"
#define AT_RST "AT+RST\r\n"
#define AT_RSTO "AT+RESTORE\r\n"
# define AT_VER "AT+GMR\r\n"

/**************** HTTP PROTOCOL **************/
#define HTTP_AUTHOR "POST /rest/api/authentications HTTP/1.1\n"
#define HTTP_AGENT "User-Agent: RESTRANGER/1.0\n"
#define HTTP_CONTENT "Content-Type: application/json\n"
#define HTTP_ACCEPT  "Accept: application/json\n"
#define HTTP_LENGTH "Content-Length: 42\n\n"
//#define AT_HTTP_START "AT+CIPSTART=\"TCP\",\"192.168.1.245\",10000\r\n"
#define AT_HTTP_START "AT+CIPSTART=\"TCP\",\"thermo-restsaves.rhcloud.com\",80\r\n"
#define AT_CIPSEND "AT+CIPSEND=%d"
#define REST_LOGIN "{\"password\": \"admin\", \"username\": \"admin\"}"

