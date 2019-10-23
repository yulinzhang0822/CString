#include <stdio.h>

#define require(e) if(!(e)) fprintf(stderr, "FAILED line %d    %s: %s    %s\n", __LINE__, __FILE__, __func__, #e)

//Computes the length of the string str up to but not including the
//terminating null character
size_t strlen(const char *str) {
    size_t length = 0;
    while(*str != '\0') {
        length++;
        str++;
    }
    return length;
}

//Compares the string pointed to, by str1 to the string pointed to by str2
int strcmp(const char *str1, const char *str2) {
    for(; *str1 == *str2; str1++, str2++) {
        if(*str1 == '\0' && *str2 == '\0') {
            return 0;
        }
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}

//Copies the string pointed to, by src to dest
char *strcpy(char *dest, const char *src) {
    char *p = dest;
    while(*src!='\0') {
        *p = *src;
        p++;
        src++;
    }
    *p = '\0';
    return dest;
}

//Appends the string pointed to, by src to the end of the string pointed to
//by dest
char *strcat(char *dest, const char *src) {
    char *p = dest;
    while(*p!='\0') {
        p++;
    }
    while(*src!='\0') {
        *p = *src;
        p++;
        src++;
    }
    *p = '\0';
    return dest;
}

//Compares at most the first n bytes of str1 and str2
int strncmp(const char *str1, const char *str2, size_t n) {
    for(size_t i =0; i<n; i++, str1++, str2++) {
        if(*str1=='\0' && *str2=='\0') {
            return 0;
        }
        if(*str1=='\0' || *str2=='\0') {
            return *str1 == '\0'? -1 : 1;
        }
        if(*str1 != *str2) {
            return *str1 - *str2 < 0?  -1 : 1;
        }
    }
    return 0;
}

//Searches for the first occurrence of the character c (an unsigned char) in
//the string pointed to, by the argument str
char *strchr(const char *str, int c) {
    char *p = (char *)str, ch = (char)c;
    while(*p!=ch) {
        p++;
        if(*p=='\0') {
            return NULL;
        }
    }
    return p;
}

//Finds the first character in the string str1 that matches any character
//specified in str2
char *strpbrk(const char *str1, const char *str2) {
    for(char *p = (char *)str1; *p!='\0'; p++) {
        for(char *q = (char *)str2; *q!='\0'; q++) {
            if(*p == *q) {
                return p;
            }
        }
    }
    return NULL;
}

//Computes the maximum number of overlap characters between the prefix and
//suffix of a C string
int partial_match_table(const char *p, int start, int end) {
    int maxValue = 0, length = end - start;
    for(int i=1; i<length-1; i++) {
        char prefix[] = "\0", suffix[] = "\0";
        for(int k=0; k<i; k++) {
            prefix[k] = p[start+k+1];
            suffix[k] = p[start+length-i+k];
        }
        int tmpValue = strcmp(prefix, suffix) == 0? i:0;
        maxValue = tmpValue>maxValue? tmpValue:maxValue;
    }
    return maxValue;
}

//KMP algorithm
char *Knuth_Morris_Pratt(const char *p, const char *q) {
    int length1 = (int)strlen(p), length2 = (int)strlen(q);
    if(length1<length2 || length1 == 0 || length2 == 0) {
        return NULL;
    }
    int i = 0, j = 0;
    while(i<length1-length2+1) {
        int start = i;
        while(p[i] == q[j]) {
            i++;
            j++;
            if(q[j]=='\0') {
                int pos = i - length2;
                while(pos-->0) {
                    p++;
                }
                return (char *)p;
            }
        }
        if(start==i) {
            i++;
            j = 0;
            continue;
        }
        int pmtValue = partial_match_table(p, start, i);
        i = start + (i - start - 1) - pmtValue;
        j = 0;
    }
    return NULL;
}

//Finds the first occurrence of the entire string needle (not including the
//terminating null character) which appears in the string haystack
char *strstr(const char *haystack, const char *needle) {
    char *p = (char *)haystack, *q = (char *)needle;
    char *ans = Knuth_Morris_Pratt(p, q);
    if(!ans) {
        return NULL;
    }
    return ans;
}

//Calculates the length of the initial segment of str1 which consists
//entirely of characters in str2
size_t strspn(const char *str1, const char *str2) {
    size_t length = 0;
    int length1 = (int)strlen(str1), length2 = (int)strlen(str2);
    if(length1<length2 || length1==0 || length2==0) {
        return 0;
    }
    char *p = (char *)str1, *q = (char *)str2;
    while(*q!='\0') {
        if(*p++ == *q++) {
            length++;
        } else {
            return 0;
        }
    }
    return length;
}

//Breaks string str into a series of tokens separated by delim
char *strtok(char *s, const char *delim) {
    char *spanp, *token;
    char ch, sc;
    static char *last;
    //check if reaches the end of s
    if (s == NULL && (s = last) == NULL) {
        return NULL;
    }
    //Skip (span) leading delimiters
    while(true) {
        ch = *s++;
        for (spanp = (char *)delim; (sc = *spanp++) != '\0';) {
            if (ch == sc) {
                break;
            }
        }
        if(sc == '\0') {
            break;
        }
    }
    //no non-delimiter characters left
    if (ch == '\0') {
        last = NULL;
        return NULL;
    }
    token = s - 1;
    //Scan token (scan for delimiters)
    while(true) {
        ch = *s++;
        spanp = (char *)delim;
        do {
            if ((sc = *spanp++) == ch) {
                if (ch == 0) {
                    s = NULL;
                }
                else {
                    s[-1] = 0;
                }
                last = s;
                return token;
            }
        } while (sc != 0);
    }
}

//The functions below are used for testing the library function defined above
void test_strlen() {
    require(strlen("") == 0);
    require(strlen(" ") == 1);//one space
    require(strlen("\t") == 1);//one tab
    require(strlen("hello") == 5);
    require(strlen("#$%&*^@!") == 8);
}

void test_strcmp() {
    char r[6] = "hello", s[6] = "hello", t[3] = "he", u[1] = "", v[2] = " ", w[2] = "\t";
    require(strcmp(r,s) == 0);
    require(strcmp(t,r) < 0);
    require(strcmp(s,t) > 0);
    require(strcmp(u,v) < 0);//empty & one space
    require(strcmp(w,v) < 0);//oen tab & one space
}


void test_strcpy() {
    char dest[20], src1[1] = "", src2[10] = "UC Irvine", src3[6] = "12345", src4[2] = " ";
    require(strcmp(strcpy(dest, src1), "") == 0 && strlen(dest) == 0);
    require(strcmp(strcpy(dest, src2), "UC Irvine") ==0 && strlen(dest) == 9);
    require(strcmp(strcpy(dest, src3), "12345") == 0 && strlen(dest) == 5);
    require(strcmp(strcpy(dest, src4), " ") == 0 && strlen(dest) == 1);
}

void test_strcat() {
    char dest[30] = "Wow", src1[3] = "Hi", src2[10] = "UC Irvine", src3[6] = "12345", src4[2] = " ";
    require(strcmp(strcat(dest, src1), "WowHi") == 0 && strlen(dest) == 5);
    require(strcmp(strcat(dest, src2), "WowHiUC Irvine") == 0 && strlen(dest) == 14);
    require(strcmp(strcat(dest, src3), "WowHiUC Irvine12345") == 0 && strlen(dest) == 19);
    require(strcmp(strcat(dest, src4), "WowHiUC Irvine12345 ") == 0 && strlen(dest) == 20);
}

void test_strncmp() {
    char r[6] = "hello", s[6] = "hello", t[3] = "he", u[1] = "", v[2] = " ", w[2] = "\t";
    require(strncmp(r,s,3) == 0);
    require(strncmp(r,s,8) == 0);
    require(strncmp(t,r,3) < 0);
    require(strncmp(s,t,3) > 0);
    require(strncmp(u,v,1) < 0);//empty & one space
    require(strncmp(w,v,1) < 0);//oen tab & one space
}

void test_strchr() {
    char r[6] = "hello", s = 'h', t = 'l', u = 'f', v = ' ', w = '\t';
    require(strcmp(strchr(r,s), "hello") == 0);
    require(strcmp(strchr(r,t), "llo") == 0);
    require(strchr(r,u) == NULL);
    require(strchr(r,v) == NULL);
    require(strchr(r,w) == NULL);
}

void test_strpbrk() {
    char r[6] = "hello", s[6] = "hello", t[3] = "fe", u[1] = "", v[2] = " ", w[2] = "\t";
    require(strcmp(strpbrk(r,s), "hello") == 0);
    require(strcmp(strpbrk(r,t), "ello") == 0);
    require(strpbrk(r,u) ==  NULL);
    require(strpbrk(r,v) ==  NULL);
    require(strpbrk(r,w) ==  NULL);
}

void test_strstr() {
    char r[30] = "BBC ABCDAB ABCDABCDABDE", s[8] = "ABCDABC", t[1] = "", u[2] = " ", v[4] = "ABF", w[2] = "\t";
    require(strcmp(strstr(r,s), "ABCDABCDABDE") == 0);
    require(strstr(r,t) == NULL);
    require(strcmp(strstr(r,u), " ABCDAB ABCDABCDABDE") == 0);
    require(strstr(r,v) == NULL);
    require(strstr(r,w) == NULL);
}

void test_strspn() {
    char r[30] = "BBC ABCDAB ABCDABCDABDE", s[12] = "BBC ABCDAB", t[1] = "", u[2] = " ", v[4] = "ABF", w[2] = "\t";
    require((int)strspn(r, s) == 10);
    require((int)strspn(r, t) == 0);
    require((int)strspn(r, u) == 0);
    require((int)strspn(r, v) == 0);
    require((int)strspn(r, w) == 0);
}

void test_strtok() {
    char str[300] = "Write at least good three test cases per string function.";
    char target[10][10] = {"Write", "at", "least", "good", "three", "test", "cases", "per", "string", "function"};
    const char s[3] = " .";
    char *token = NULL;
    //get the first token
    token = strtok(str, s);
    require(strcmp(token, target[0]) == 0);
    int i = 1;
    while((token = strtok(NULL, s)) != NULL) {
        require(strcmp(token, target[i++]) == 0);
    }
}

//The program will print out where the errors occur if there are bugs in the defined function library; 
//otherwise, nothing will prompt up
int main() {
    test_strlen();
    test_strcmp();
    test_strcpy();
    test_strcat();
    test_strncmp();
    test_strchr();
    test_strpbrk();
    test_strstr();
    test_strspn();
    test_strtok();
    return 0;
}
