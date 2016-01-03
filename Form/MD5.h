#ifndef MD5_H_H
#define MD5_H_H

void MD5_Reset();
//void MD5_Core(const unsigned char block[64]);
int MD5_Buffer(const unsigned char *in_buffer, const unsigned int size, bool last_buffer);
int MD5_File(const char *file_path);
void MD5_GetResult(char *out_buffer);

#endif /* MD5_H_H */