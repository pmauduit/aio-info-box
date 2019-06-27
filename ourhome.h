#ifndef __OURHOME_H
#define __OURHOME_H

#include <curl/curl.h>
#include <list>

size_t
write_cb(void *, size_t, size_t, void *);

size_t
discard_cb(void *, size_t, size_t, void *);

#define OURHOME_LOGIN_URL  "https://api.ourhomeapp.com/api/v1/users/login/"
#define OURHOME_LOGOUT_URL "https://api.ourhomeapp.com/api/v1/users/logout/"
#define OURHOME_CHORES_URL "https://api.ourhomeapp.com/api/v1/tasks/?limit=25&is_active=true&sorting=true&is_event=false"
#define OURHOME_SHOPPING_URL "https://api.ourhomeapp.com/api/v1/shopping_items/?limit=25&is_active=true&sorting=true"

class OurHome {


  public:
    OurHome();
    ~OurHome();
    void login(std::string username, std::string password);
    void logout();
    std::list<std::string> getChores();
    std::list<std::string> getShoppingList();

  private:
    std::string sessionid;
    CURL * curlCtx = NULL;

    const static std::string loginUrl;
    const static std::string logoutUrl;
    const static std::string choresUrl;
    const static std::string shoppingUrl;

    void extractSessionId(std::stringbuf *);
    std::string doAuthenticatedRequest(const std::string &url);

};

#endif

