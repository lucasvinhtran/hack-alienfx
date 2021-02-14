#include "functions.h"

/*
 * This method sends a line to the keyboard
 * Returns:
 *   0 - On success
 *   4 - On invalid line
 *   5 - On error sending the line
 */
int doTheThing(char* line){
  int ret;
  unsigned char buf[9];

  ret = parse_bytes(line, buf);
  if(ret != 0) {
    return 4;
  }
  usleep(150);
  ret = send_message(buf);
  if(ret != 0) {
    return 5;
  }
  return 0;
}

int clear(){
  	libusb_device_handle *dev;
	int ret;

	ret = libusb_init(NULL);
	if(ret != 0) {
		fprintf(stderr, "failed to initialize libusb\n");
		return 1;
	}

	// DEBUG
	libusb_set_debug(NULL, LIBUSB_LOG_LEVEL_DEBUG);

	dev = libusb_open_device_with_vid_pid(NULL, 0x187c, 0x0530);
	if(dev == NULL) {
		fprintf(stderr, "failed to open usb device\n");
		return 1;
	}

	libusb_detach_kernel_driver(dev, 0);
	libusb_reset_device(dev);
	libusb_close(dev);

	libusb_exit(NULL);
	return 0;
}

int main(int argc, char** argv){
  //printf("The number of arguments is: %d\n", argc);
  if ((argc==2)){    
    //printf("The file is: \n");
    //printf("  <%s>\n", argv[1]);
    FILE *file;
    if ((file = fopen(argv[1], "r"))) {
	if (init_device() != 0){
	  /*
	   * It may not be closed
	   */
	  printf(" + Error opening device\n");
	  return clear();
	} else {
	  char* line = NULL;
	  char* clean = NULL;
	  size_t len = 0;
	  ssize_t read;
	  printf("< Doing the thing >\n");
	  while ((read = getline(&line, &len, file)) != -1) {	    
	      if (line==NULL || !strcmp(line, "\n")) {
		printf(" - Empty line: %s",line);
	      } else {
		clean = trim(line);
		if ((clean[0]=='#')){
		  printf(" - Comment: %s\n", clean);
		} else {
		  switch (doTheThing(clean)){
		    case 0:
		      printf(" - %s\n", clean);
		      break;
		    case 4:
		      printf(" + Invalid line: %s\n", clean);
		      break;
		    case 5:
		      printf(" + Cannot send message: %s\n", clean);
		      break;
		    default:
		      printf(" + This was unexpected and not my fault\n");
		      fclose(file);
		      deinit_device();
		      return -1;
		  }
		}
	      }
	  }
	  printf("</Doing the thing>\n");
	  fclose(file);
	  deinit_device();
	  return 0;
	}
    } else {
      printf(" -| MINIMAL AlienFX\n");
      printf(" + Invalid file: Not found");
      return 2;
    }
  } else {
    printf(" -| MINIMAL AlienFX\n");
    printf(" + Invalid arguments\n");
    printf(" + Must especify a file\n");
    return 1;
  }
}