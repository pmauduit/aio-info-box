#ifndef __OURHOME_H
#define __OURHOME_H

#include <list>

size_t
write_cb(void *, size_t, size_t, void *);

size_t
discard_cb(void *, size_t, size_t, void *);

#define OURHOME_LOGIN_URL  "https://api.ourhomeapp.com/api/v1/users/login/"
#define OURHOME_CHORES_URL "https://api.ourhomeapp.com/api/v1/tasks/?limit=23&is_active=true&sorting=true&is_event=false"


class OurHome {


  public:
    OurHome();
    ~OurHome();
    void login(std::string username, std::string password);
    std::list<std::string> getChores();

  private:
    std::string sessionid;
    CURL * curlCtx = NULL;
    std::list<std::string> chores;

    const static std::string loginUrl;
    const static std::string choresUrl;

    void extractSessionId(std::stringbuf *);

};

#endif

