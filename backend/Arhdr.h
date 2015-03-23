#ifndef BACKEND_ARHDR_H
#define BACKEND_ARHDR_H

#include <cstdlib>

#include "common.h"

BEGIN_BIN_NAMESPACE(backend)

class Arhdr {
public:
    Arhdr(const void * = NULL, loff_t = 0);
    Arhdr(const Arhdr &);
    Arhdr &operator=(const Arhdr &);
    ~Arhdr();
    bool isValid() { return _valid; }
    const char *name() { return _name; }
    time_t date() { return _date; }
    uid_t uid() { return _uid; }
    gid_t gid() { return _gid; }
    mode_t mode() { return _mode; }
    loff_t size() { return _size; }
    const char *rawName() { return _rawname; }
    loff_t offset() { return _offset; }
private:
    bool _valid;
    char *_name;		/* Name of archive member.  */
    time_t _date;		/* File date.  */
    uid_t _uid;			/* User ID.  */
    gid_t _gid;			/* Group ID.  */
    mode_t _mode;		/* File mode.  */
    loff_t _size;		/* File size.  */
    char *_rawname;		/* Original name of archive member.  */
    loff_t _offset;
};

END_BIN_NAMESPACE

#endif
