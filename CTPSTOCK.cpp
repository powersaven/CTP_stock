// CTPSTOCK.cpp : Defines the entry point for the console application.

#include "util.h"
#include "ZQTD.h"
#include "ZQMD.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <event.h>
#include <event2/event.h>
#ifdef _EVENT___func__
#define __func__ _EVENT___func__
#endif

#include "libmysql/include/mysql.h"

MYSQL *mysql_fd = NULL;
MYSQL *mysql = NULL;

static void signal_cb(evutil_socket_t fd, short event, void *arg) {
    struct event *signal = (struct event *)arg;
    sleep(10);
    printf("%s: got signal %d\n", __func__, EVENT_SIGNAL(signal));
}

int main(int argc, char* argv[])
{
    
//    CZQTD * pTdHandler=new CZQTD();
//    pTdHandler->Init();

    if (mysql_library_init(0, NULL, NULL)) {
        printf("Could not initialize mysql library\n");
        exit(EXIT_FAILURE);
    }

    mysql_fd = mysql_init(NULL);
    mysql = mysql_real_connect(mysql_fd, "192.168.1.101", "ctp", "ctp", "ctp", 3307, NULL, 0);
 
    if (NULL == mysql) {
        printf("%s\n", mysql_error(mysql_fd));
        mysql_close(mysql);

        return 1;
    }

    CZQMD * pMdHandler=new CZQMD();
    pMdHandler->Init();
 
    struct event signal_usr;
    struct event_base* base;

    /* Initalize the event library */
    base = event_base_new();
    /* Initalize one event */
    event_assign(&signal_usr, base, SIGUSR1, EV_SIGNAL|EV_PERSIST, signal_cb, &signal_usr);
    event_add(&signal_usr, NULL);
    event_base_dispatch(base);
    event_base_free(base);

    return 0;
}

