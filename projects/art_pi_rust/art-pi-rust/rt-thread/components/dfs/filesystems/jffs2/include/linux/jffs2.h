/*
 * JFFS2 -- Journalling Flash File System, Version 2.
 *
 * Copyright (C) 2001-2003 Red Hat, Inc.
 *
 * Created by David Woodhouse <dwmw2@infradead.org>
 *
 * For licensing information, see the file 'LICENCE' in the
 * jffs2 directory.
 *
 * $Id: jffs2.h,v 1.36 2005/07/26 13:19:36 havasi Exp $
 *
 */

#ifndef __LINUX_JFFS2_H__
#define __LINUX_JFFS2_H__

/* You must include something which defines the C99 uintXX_t types.
   We don't do it from here because this file is used in too many
   different environments. */

#define JFFS2_SUPER_MAGIC 0x72b6

/* Values we may expect to find in the 'magic' field */
#define JFFS2_OLD_MAGIC_BITMASK 0x1984
#define JFFS2_MAGIC_BITMASK 0x1985
#define KSAMTIB_CIGAM_2SFFJ 0x8519 /* For detecting wrong-endian fs */
#define JFFS2_EMPTY_BITMASK 0xffff
#define JFFS2_DIRTY_BITMASK 0x0000

/* We only allow a single char for length, and 0xFF is empty flash so
   we don't want it confused with a real length. Hence max 254.
*/
#define JFFS2_MAX_NAME_LEN 254

/* How small can we sensibly write nodes? */
#define JFFS2_MIN_DATA_LEN 128

#define JFFS2_COMPR_NONE	0x00
#define JFFS2_COMPR_ZERO	0x01
#define JFFS2_COMPR_RTIME	0x02
#define JFFS2_COMPR_RUBINMIPS	0x03
#define JFFS2_COMPR_COPY	0x04
#define JFFS2_COMPR_DYNRUBIN	0x05
#define JFFS2_COMPR_ZLIB	0x06
/* Compatibility flags. */
#define JFFS2_COMPAT_MASK 0xc000      /* What do to if an unknown nodetype is found */
#define JFFS2_NODE_ACCURATE 0x2000
/* INCOMPAT: Fail to mount the filesystem */
#define JFFS2_FEATURE_INCOMPAT 0xc000
/* ROCOMPAT: Mount read-only */
#define JFFS2_FEATURE_ROCOMPAT 0x8000
/* RWCOMPAT_COPY: Mount read/write, and copy the node when it's GC'd */
#define JFFS2_FEATURE_RWCOMPAT_COPY 0x4000
/* RWCOMPAT_DELETE: Mount read/write, and delete the node when it's GC'd */
#define JFFS2_FEATURE_RWCOMPAT_DELETE 0x0000

#define JFFS2_NODETYPE_DIRENT (JFFS2_FEATURE_INCOMPAT | JFFS2_NODE_ACCURATE | 1)
#define JFFS2_NODETYPE_INODE (JFFS2_FEATURE_INCOMPAT | JFFS2_NODE_ACCURATE | 2)
#define JFFS2_NODETYPE_CLEANMARKER (JFFS2_FEATURE_RWCOMPAT_DELETE | JFFS2_NODE_ACCURATE | 3)
#define JFFS2_NODETYPE_PADDING (JFFS2_FEATURE_RWCOMPAT_DELETE | JFFS2_NODE_ACCURATE | 4)

// Maybe later...
//#define JFFS2_NODETYPE_CHECKPOINT (JFFS2_FEATURE_RWCOMPAT_DELETE | JFFS2_NODE_ACCURATE | 3)
//#define JFFS2_NODETYPE_OPTIONS (JFFS2_FEATURE_RWCOMPAT_COPY | JFFS2_NODE_ACCURATE | 4)


#define JFFS2_INO_FLAG_PREREAD	  1	/* Do read_inode() for this one at
					   mount time, don't wait for it to
					   happen later */
#define JFFS2_INO_FLAG_USERCOMPR  2	/* User has requested a specific
					   compression type */


/* These can go once we've made sure we've caught all uses without
   byteswapping */
#include <stdint.h>

#if defined(__GNUC__) || (__CC_ARM) 
typedef struct {
	uint32_t v32;
} __attribute__((packed))  jint32_t;

typedef struct {
	uint32_t m;
} __attribute__((packed))  jmode_t;

typedef struct {
	uint16_t v16;
} __attribute__((packed)) jint16_t;

struct jffs2_unknown_node
{
	/* All start like this */
	jint16_t magic;
	jint16_t nodetype;
	jint32_t totlen; /* So we can skip over nodes we don't grok */
	jint32_t hdr_crc;
} __attribute__((packed));

struct jffs2_raw_dirent
{
	jint16_t magic;
	jint16_t nodetype;	/* == JFFS2_NODETYPE_DIRENT */
	jint32_t totlen;
	jint32_t hdr_crc;
	jint32_t pino;
	jint32_t version;
	jint32_t ino; /* == zero for unlink */
	jint32_t mctime;
	uint8_t nsize;
	uint8_t type;
	uint8_t unused[2];
	jint32_t node_crc;
	jint32_t name_crc;
	uint8_t name[0];
} __attribute__((packed));

/* The JFFS2 raw inode structure: Used for storage on physical media.  */
/* The uid, gid, atime, mtime and ctime members could be longer, but
   are left like this for space efficiency. If and when people decide
   they really need them extended, it's simple enough to add support for
   a new type of raw node.
*/
struct jffs2_raw_inode
{
	jint16_t magic;      /* A constant magic number.  */
	jint16_t nodetype;   /* == JFFS2_NODETYPE_INODE */
	jint32_t totlen;     /* Total length of this node (inc data, etc.) */
	jint32_t hdr_crc;
	jint32_t ino;        /* Inode number.  */
	jint32_t version;    /* Version number.  */
	jmode_t mode;       /* The file's type or mode.  */
	jint16_t uid;        /* The file's owner.  */
	jint16_t gid;        /* The file's group.  */
	jint32_t isize;      /* Total resultant size of this inode (used for truncations)  */
	jint32_t atime;      /* Last access time.  */
	jint32_t mtime;      /* Last modification time.  */
	jint32_t ctime;      /* Change time.  */
	jint32_t offset;     /* Where to begin to write.  */
	jint32_t csize;      /* (Compressed) data size */
	jint32_t dsize;	     /* Size of the node's data. (after decompression) */
	uint8_t compr;       /* Compression algorithm used */
	uint8_t usercompr;   /* Compression algorithm requested by the user */
	jint16_t flags;	     /* See JFFS2_INO_FLAG_* */
	jint32_t data_crc;   /* CRC for the (compressed) data.  */
	jint32_t node_crc;   /* CRC for the raw inode (excluding data)  */
	uint8_t data[0];
} __attribute__((packed));

#elif defined (MSVC)
typedef uint32_t jint32_t;
//typedef uint32_t jmode_t;
typedef struct {
	uint32_t m;
} jmode_t;
typedef uint16_t jint16_t;

#pragma pack(1)
struct jffs2_unknown_node
{
	/* All start like this */
	jint16_t magic;
	jint16_t nodetype;
	jint32_t totlen; /* So we can skip over nodes we don't grok */
	jint32_t hdr_crc;
}; 

struct jffs2_raw_dirent
{
	jint16_t magic;
	jint16_t nodetype;	/* == JFFS2_NODETYPE_DIRENT */
	jint32_t totlen;
	jint32_t hdr_crc;
	jint32_t pino;
	jint32_t version;
	jint32_t ino; /* == zero for unlink */
	jint32_t mctime;
	uint8_t nsize;
	uint8_t type;
	uint8_t unused[2];
	jint32_t node_crc;
	jint32_t name_crc;
	uint8_t name[0];
};

/* The JFFS2 raw inode structure: Used for storage on physical media.  */
/* The uid, gid, atime, mtime and ctime members could be longer, but
   are left like this for space efficiency. If and when people decide
   they really need them extended, it's simple enough to add support for
   a new type of raw node.
*/
struct jffs2_raw_inode
{
	jint16_t magic;      /* A constant magic number.  */
	jint16_t nodetype;   /* == JFFS2_NODETYPE_INODE */
	jint32_t totlen;     /* Total length of this node (inc data, etc.) */
	jint32_t hdr_crc;
	jint32_t ino;        /* Inode number.  */
	jint32_t version;    /* Version number.  */
	jmode_t mode;       /* The file's type or mode.  */
	jint16_t uid;        /* The file's owner.  */
	jint16_t gid;        /* The file's group.  */
	jint32_t isize;      /* Total resultant size of this inode (used for truncations)  */
	jint32_t atime;      /* Last access time.  */
	jint32_t mtime;      /* Last modification time.  */
	jint32_t ctime;      /* Change time.  */
	jint32_t offset;     /* Where to begin to write.  */
	jint32_t csize;      /* (Compressed) data size */
	jint32_t dsize;	     /* Size of the node's data. (after decompression) */
	uint8_t compr;       /* Compression algorithm used */
	uint8_t usercompr;   /* Compression algorithm requested by the user */
	jint16_t flags;	     /* See JFFS2_INO_FLAG_* */
	jint32_t data_crc;   /* CRC for the (compressed) data.  */
	jint32_t node_crc;   /* CRC for the raw inode (excluding data)  */
	uint8_t data[0];
};
#pragma pack()
#else
#endif


union jffs2_node_union {
	struct jffs2_raw_inode i;
	struct jffs2_raw_dirent d;
	struct jffs2_unknown_node u;
};

#endif /* __LINUX_JFFS2_H__ */
