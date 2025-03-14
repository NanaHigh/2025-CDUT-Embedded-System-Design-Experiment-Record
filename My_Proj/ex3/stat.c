int stat(char* p, char ch) {
    int i = 0;
    int num = 0;

    for (i = 0; *(p + i) != '\0'; i++) {
        if (*(p + i) == ch) {
            num++;
        }
    }
    
    return num;
}