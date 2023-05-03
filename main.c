#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <lauxlib.h>
#include <security/pam_appl.h>


static int simple (int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr) {
  struct pam_response *response;

  if ( num_msg != 1 )
    return PAM_BUF_ERR;

  response = malloc (sizeof (struct pam_response));
  if ( response == NULL )
    return PAM_BUF_ERR;

  response->resp = strdup (appdata_ptr);
  if ( response->resp == NULL ) {
    free (response);
    return PAM_BUF_ERR;
  }
  response->resp_retcode = 0;
 
  *resp = response;
  return PAM_SUCCESS;
}

static int auth(lua_State *L) {
  int result;
  const char *pass = luaL_checkstring (L, -1);

  struct pam_conv conv = { simple, (void *) pass };
  pam_handle_t *pamh = NULL;
 
  uid_t uid = getuid ();
  struct passwd *pw = getpwuid (uid);

  result = pam_start ("login", pw->pw_name, &conv, &pamh);
  result = pam_authenticate (pamh, result);
  pam_end (pamh, 0);

  lua_pushboolean(L, result == PAM_SUCCESS);
  return 1;
}

static const struct luaL_Reg lua_pam[] = {
  { "auth", auth },
  { NULL, NULL }  /* sentinel */
};

extern int luaopen_libpam(lua_State *L) {
  luaL_newlib(L, lua_pam);
  return 1;
}
