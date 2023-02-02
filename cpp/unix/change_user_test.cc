#include <unistd.h>
// root 身份启动，普通身份运行
static bool switch_to_user(uid_t user_id, gid_t gp_id) {
  if (user_id == 0 && gp_id == 0) {
    // root 返回 false
    return false;
  }

  gid_t gid = getgid();
  uid_t uid = getuid();
  if ((gid != 0 || uid != 0) && (gid != gp_id || uid != user_id)) {
    return false;
  }

  if (uid != 0) {
    return true;
  }

  if (setgid(gp_id) < 0 || setuid(user_id) < 0) {
    return false;
  }

  return true;
}

int main() {
  switch_to_user(0, 0);
  return 0;
}