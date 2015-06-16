#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define VERSION "0.1"

#define DEF_MAXSTREAMS          10  /* Maximum number of webcam clients per camera */
#define DEF_MAXWEBQUEUE         10  /* Maximum number of webcam client in queue */

#define DEF_TIMESTAMP           "%Y-%m-%d\\n%T"
#define DEF_EVENTSTAMP          "%Y%m%d%H%M%S"

#define VIDEO_PALETTE_GREY      1       /* Linear greyscale */
#define VIDEO_PALETTE_HI240     2       /* High 240 cube (BT848) */
#define VIDEO_PALETTE_RGB565    3       /* 565 16 bit RGB */
#define VIDEO_PALETTE_RGB24     4       /* 24bit RGB */
#define VIDEO_PALETTE_RGB32     5       /* 32bit RGB */
#define VIDEO_PALETTE_RGB555    6       /* 555 15bit RGB */
#define VIDEO_PALETTE_YUV422    7       /* YUV422 capture */
#define VIDEO_PALETTE_YUYV      8
#define VIDEO_PALETTE_UYVY      9       /* The great thing about standards is ... */
#define VIDEO_PALETTE_YUV420    10
#define VIDEO_PALETTE_YUV411    11      /* YUV411 capture */
#define VIDEO_PALETTE_RAW       12      /* RAW capture (BT848) */
#define VIDEO_PALETTE_YUV422P   13      /* YUV 4:2:2 Planar */
#define VIDEO_PALETTE_YUV411P   14      /* YUV 4:1:1 Planar */
#define VIDEO_PALETTE_YUV420P   15      /* YUV 4:2:0 Planar */
#define VIDEO_PALETTE_YUV410P   16      /* YUV 4:1:0 Planar */
#define VIDEO_PALETTE_PLANAR    13      /* start of planar entries */
#define VIDEO_PALETTE_COMPONENT 7       /* start of component entries */

#define motion_log(lvl, i, fmt, args...) fprintf(stderr, fmt "\n", ##args)
#define mymalloc malloc

#ifdef __GNUC__
#ifdef HAVE_ANSIDECL_H
#include <ansidecl.h>
#endif
#ifndef ATTRIBUTE_UNUSED
#define ATTRIBUTE_UNUSED __attribute__((unused))
#endif
#else
#define ATTRIBUTE_UNUSED
#endif

#define CAMERA_WARNINGS         3   /* warnings only */
#define CAMERA_INFO             5   /* info debug */
#define CAMERA_VIDEO            6   /* video debug */
#define CAMERA_DEBUG            7   /* debug but not verbose */
#define CAMERA_VERBOSE          8   /* verbose level */
#define CAMERA_ALL              9   /* everything */



/*
 * webcam stuff start
 */
struct webcam_buffer {
    unsigned char *ptr;
    int ref;
    long size;
};

struct webcam {
    int socket;
    FILE *fwrite;
    struct webcam_buffer *tmpbuffer;
    long filepos;
    int nr;
    unsigned long int last;
    struct webcam *prev;
    struct webcam *next;
};

/*
 * image stuff start
 */
struct images {
    int width;
    int height;
    int type; //VIDEO_PALETTE_YUV420P:
    int size;
    unsigned char *common_buffer;
    int motionsize;
};


/*
 * generic stuff start
 */
struct config {
	int width;
	int height;

	int setup_mode; //0
	int webcam_limit; //0 - no limit
	int webcam_port; //8081
	int webcam_localhost; //0 - do not restrict to localhost
	int webcam_quality; //50 - from 0~100
	int webcam_maxrate; //30 - fps
	int v4l2_palette; //8
		/*
		# Values :
		# V4L2_PIX_FMT_SN9C10X : 0  'S910'
		# V4L2_PIX_FMT_SBGGR8  : 1  'BA81'
		# V4L2_PIX_FMT_MJPEG   : 2  'MJPEG'
		# V4L2_PIX_FMT_JPEG    : 3  'JPEG'
		# V4L2_PIX_FMT_RGB24   : 4  'RGB3'
		# V4L2_PIX_FMT_UYVY    : 5  'UYVY'
		# V4L2_PIX_FMT_YUYV    : 6  'YUYV'
		# V4L2_PIX_FMT_YUV422P : 7  '422P'
		# V4L2_PIX_FMT_YUV420  : 8  'YU12'
		*/
	int autobright; 
		/*
 		# Let motion regulate the brightness of a video device (default: off).
		# The auto_brightness feature uses the brightness option as its target value.
		# If brightness is zero auto_brightness will adjust to average brightness value 128.
		# Only recommended for cameras without auto brightness
		*/
	int brightness; 
		/*
		# Set the initial brightness of a video device.
		# If auto_brightness is enabled, this value defines the average brightness level
		# which Motion will try and adjust to.
		# Valid range 0-255, default 0 = disabled
		*/
	int contrast;
		/*
		# Set the contrast of a video device.
		# Valid range 0-255, default 0 = disabled
		*/
	int saturation;
		/*
		# Set the saturation of a video device.
		# Valid range 0-255, default 0 = disabled
		*/
	int hue;
		/*
		# Set the hue of a video device (NTSC feature).
		# Valid range 0-255, default 0 = disabled
		*/
	int frame_limit; //30 - fps
	int input; //8
		/*
		# The video input to be used (default: 8)
		# Should normally be set to 0 or 1 for video/TV cards, and 8 for USB cameras
		*/
	int norm; // 0
		/*
		# The video norm to use (only for video capture and TV tuner cards)
		# Values: 0 (PAL), 1 (NTSC), 2 (SECAM), 3 (PAL NC no colour). Default: 0 (PAL)
		*/
	int roundrobin_frames; // 1
		/*
		# Number of frames to capture in each roundrobin step (default: 1)
		*/	
	int roundrobin_skip; // 1
		/*
 		# Number of frames to skip before each roundrobin step (default: 1)
		*/
	int frequency; //  0
		/*
 		# The frequency to set the tuner to (kHz) (only for TV tuner cards) (default: 0)
		*/
	int tuner_number; // 0 - only used for BSD
	const char* video_device; // /dev/video0
};

struct context {
	int stream_count;
	int video_dev;

	struct config conf;
	struct webcam webcam;
	struct images imgs;
};

struct video_dev {
    struct video_dev *next;
    int usage_count;
    int fd;
    const char *video_device;
    int input;
    int width;
    int height;
    int brightness;
    int contrast;
    int saturation;
    int hue;
    unsigned long freq;
    int tuner_number;
    int fps;

    pthread_mutex_t mutex;
    pthread_mutexattr_t attr;
    int owner;
    int frames;

    /* Device type specific stuff: */
#ifndef WITHOUT_V4L
    /* v4l */
    int v4l2;
    void *v4l2_private;

    int size_map;
    int v4l_fmt;
    unsigned char *v4l_buffers[2];
    int v4l_curbuffer;
    int v4l_maxbuffer;
    int v4l_bufsize;
#endif
};

/*
 * function definitions
 */
int webcam_init(struct context *);
void webcam_put(struct context *, unsigned char *);
void webcam_stop(struct context *);

int put_jpeg_yuv420p_memory(unsigned char *dest_image, int image_size,
    unsigned char *input_image, int width, int height, int quality);

extern unsigned short int debug_level;

void vid_init(void);
void vid_cleanup(void);

int vid_v4l2_start(struct context *cnt);
int vid_next(struct context* cnt, unsigned char* map);

void vid_close(struct context *cnt);

int put_picture_memory(struct context *cnt, unsigned char* dest_image, int image_size,
                       unsigned char *image, int quality);

int v4l2_next(struct context *cnt, struct video_dev *viddev, unsigned char *map, int width, int height);

#endif
