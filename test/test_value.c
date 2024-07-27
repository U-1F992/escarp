#include <escarp.h>

#include <assert.h>
#include <stdio.h>

int main(void) {
    escarp_error_t err = ESCARP_SUCCESS;

    escarp_value_t value[256];
    size_t value_usage = 0;

    FILE *fp = NULL;
    int out[256];

    escarp_parser_t *parser = NULL;

    parser = escarp_value(NULL, 0);
    assert(NULL == parser);

    parser = escarp_value(&value[value_usage++], 0);
    assert(NULL != parser);

    fp = tmpfile();
    err = escarp_parse(parser, fp, out);
    assert(ESCARP_ERROR_UNEXPECTED_EOF == err);
    fclose(fp);

    fp = tmpfile();
    fwrite((unsigned char[]){0}, 1, 1, fp);
    fseek(fp, 0, SEEK_SET);
    err = escarp_parse(parser, fp, out);
    assert(ESCARP_SUCCESS == err);
    assert(0 == out[0]);
    assert(EOF == out[1]);
    fclose(fp);

    fp = tmpfile();
    fwrite((unsigned char[]){1}, 1, 1, fp);
    fseek(fp, 0, SEEK_SET);
    err = escarp_parse(parser, fp, out);
    assert(ESCARP_ERROR_UNEXPECTED_VALUE == err);
    fclose(fp);

    fp = tmpfile();
    fwrite((unsigned char[]){0, 1}, 1, 2, fp);
    fseek(fp, 0, SEEK_SET);
    err = escarp_parse(parser, fp, out);
    assert(ESCARP_SUCCESS == err);
    assert(0 == out[0]);
    assert(EOF == out[1]);
    fclose(fp);

    parser = escarp_value(&value[value_usage++], 0x80);
    assert(NULL != parser);
    fp = tmpfile();
    fwrite((unsigned char[]){0x80}, 1, 1, fp);
    fseek(fp, 0, SEEK_SET);
    err = escarp_parse(parser, fp, out);
    assert(ESCARP_SUCCESS == err);
    assert(0x80 == out[0]);
    assert(EOF == out[1]);
    fclose(fp);
    
    return 0;
}
