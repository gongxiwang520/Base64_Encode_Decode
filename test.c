#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


/*
Base64转换为16进制数据在线计算：https://the-x.cn/base64/
*/


// Base64编码表
char Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"\
               "abcdefghijklmnopqrstuvwxyz"\
               "0123456789+/";


/**
 * @brief 字符串转换为对应的Base64编码
 * @param dst：转换后的Base64字符串   
 * @param src：转换前的字符串
 * @param length：转换前字符串长度
 * @note  Base64总共有64个ASCII，包括A-Z,a-z,0-9,"+"和"/"
 */
void EncodeBase64(char *dst, char *src, int length)
{   
    int j = 0;
    int i = 0;
    for (i=0; i<length; i++)
    {       
        j = i / 3 * 4;
        if (i % 3 == 0)
        {
            if ((length - i) < 3 && (length - i) > 0)
            {// 不足3字节的转换后末尾补=
                dst[j] = Table[((src[i] & 0xFC) >> 2) & 0x3F];
                dst[j + 1] = Table[(((src[i] & 0x03) << 4) | ((src[i + 1] & 0xF0) >> 4)) & 0x3F];
                switch (length%3)
                {
                case 1:
                    dst[j + 2] = '=';
                    dst[j + 3] = '=';
                    break;
                case 2:
                    dst[j + 2] = Table[((src[i + 1] & 0x0F)<<2) & 0x3F];;
                    dst[j + 3] = '=';
                    break;
                default:                    
                    break;
                }
                break;  // 跳出整个for循环
            }           
            // 3字节转为4字节
            dst[j] = Table[((src[i] & 0xFC) >> 2) & 0x3F];
            dst[j + 1] = Table[(((src[i] & 0x03) << 4) | ((src[i + 1] & 0xF0) >> 4)) & 0x3F];
            dst[j + 2] = Table[(((src[i + 2] & 0xC0) >> 6) & 0x03) | (((src[i + 1] & 0x0F) << 2))];
            dst[j + 3] = Table[(src[i + 2] & 0x3F) & 0x3F];
        }               
    }
}

/**
 * @brief   在Table查找字符temp对应的位置
 * @param   temp：待查找的字符
 * @retval  字符在编码表中的位置0-63
 * @note    
*/
char FindHexCode(char temp)
{
    char *pbuff = Table;
    int index = 0;
    while (*pbuff != '=')
    {
        if (temp == pbuff[index])
        {
            break;
        }
        index++;
    }
    return index;
}

/**
 * @brief Base64编码转换为字符串
 * @param dst：转换后的字符串  
 * @param src：待转换的Base64数据
 * @param length：待转换的长度
 * @note
*/
void DecodeBase64(char *dst,char *src, int length)
{
    int i = 0;
    int j = 0;
    char temp[4] = { 0 };
    for (i=0;i<length;i++)
    {
        j = i / 4 * 3;
        if ( i%4 == 0 )
        {
            // 先将Base64字符转换为索引值
            for (int k=0;k<4;k++)
            {
                temp[k] = FindHexCode(src[i + k]);
            }
            // 4字节转换为3字节
            dst[j] = ((temp[0] << 2) & 0xFC) | ((temp[1] >> 4) & 0x03);
            dst[j + 1] = ((temp[1] << 4) & 0xF0) | ((temp[2] >> 2) & 0x0F);
            dst[j + 2] = ( (temp[2]<<6) & 0xC0) | temp[3];
        }       
    }
}


int main(int argc, char *argv[])
{
  char buff[1024] = {0};
  char test[] = "www.greywalltech.com";
  char result[1024] = {0};

  EncodeBase64(buff, test, strlen(test));
  printf("Base64 Encode: %s\r\n", buff);

  DecodeBase64(result, buff, strlen(buff));
  printf("Base64 Decode: %s\n", result);
}






