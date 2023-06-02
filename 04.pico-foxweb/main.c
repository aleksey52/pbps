#include "httpd.h"
#include <sys/stat.h>
#include <pwd.h>
#include <stdlib.h>
#include <dirent.h>

#define CHUNK_SIZE 1024 // read 1024 bytes at a time

// Public directory settings
#define INDEX_HTML "/index.html"
#define NOT_FOUND_HTML "/404.html"

int main(int c, char **v) {
  char *port = c <= 1 ? "8000" : v[1];
  public_dir = c <= 2 ? "./webroot" : v[2];
  char *username = c <=3 ? "picofoxweb" : v[3];
  struct passwd *p;
  if ((p = getpwnam(username)) == NULL) {
      perror(username);
      exit(1);
  }
  uid = p->pw_uid;
  printf("%d", uid);
  serve_forever(port);
  return 0;
}

int file_exists(const char *file_name) {
  struct stat buffer;
  int exists;

  exists = (stat(file_name, &buffer) == 0);

  return exists;
}

int read_file(const char *file_name) {
  char buf[CHUNK_SIZE];
  FILE *file;
  size_t nread;
  int err = 1;

  file = fopen(file_name, "r");

  if (file) {
    while ((nread = fread(buf, 1, sizeof buf, file)) > 0)
      fwrite(buf, 1, nread, stdout);

    err = ferror(file);
    sprintf(responseSize, "%ld", ftell(file));
    fclose(file);
  }
  return err;
}

void route() {
  ROUTE_START()

  GET("/")
    char index_html[40];
    sprintf(index_html, "%s", INDEX_HTML);

    HTTP_200
    if (file_exists(index_html)) {
      read_file(index_html);
    } else {
      printf("Hello! You are using %s\n\n", request_header("User-Agent"));
    }

  GET("/test")
    HTTP_200
    printf("List of request headers:\n\n");

    header_t *h = request_headers();

    while (h->name) {
      printf("%s: %s\n", h->name, h->value);
      h++;
    }

  GET("/list")
    HTTP_200
    printf("List of files and dirs in root:\n\n");

    DIR *d;
    struct dirent *dir;
    d = opendir("/");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }

  POST("/")
    HTTP_201
    printf("Wow, seems that you POSTed %d bytes.\n", payload_size);
    printf("Fetch the data using `payload` variable.\n");
    if (payload_size > 0)
      printf("Request body: %s", payload);

  GET(uri)
    char file_name[255];
    sprintf(file_name, "%s", uri);

    if (file_exists(file_name)) {
      HTTP_200
      read_file(file_name);
    } else {
      HTTP_404;
      sprintf(file_name, "%s",  NOT_FOUND_HTML);
      if (file_exists(file_name))
        read_file(file_name);
    }

  ROUTE_END()
}
