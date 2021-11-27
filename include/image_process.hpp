#ifndef __IMAGE_PROCESS_H__
#define __IMAGE_PROCESS_H__

#ifdef __cplusplus
extern "C" {
#endif

void grayscale_file(const char* in_file_path, const char* out_file_path);

void edge_detect_file(const char* in_file_path, const char* out_file_path, int low_threshold, int high_threshold, int kernel_size);

void blur_file(const char* in_file_path, const char* out_file_path, int kernel_size);

#ifdef __cplusplus
} //end extern c
#endif

#endif /* __IMAGE_PROCESS_H__ */
