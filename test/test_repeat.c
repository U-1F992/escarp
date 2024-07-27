#include <escarp.h>

#include <assert.h>
#include <stdio.h>

int main(void) {
    escarp_error_t err = ESCARP_SUCCESS;

    escarp_any_t any[256];
    size_t any_usage = 0;

    escarp_value_t value[256];
    size_t value_usage = 0;

    escarp_sequence_t sequence[256];
    size_t sequence_usage = 0;

    escarp_repeat_t repeat[256];
    size_t repeat_usage = 0;

    FILE *fp = NULL;
    int out[256];

    escarp_parser_t *parser = NULL;

    parser = escarp_repeat(NULL, NULL, 1, 0);
    assert(NULL == parser);
    parser = escarp_repeat(&repeat[repeat_usage++], NULL, 1, 0);
    assert(NULL == parser);
    parser = escarp_repeat(&repeat[repeat_usage++],
                           escarp_value(&value[value_usage++], 0x80), 1, 0);
    assert(NULL == parser);

    parser = escarp_repeat(&repeat[repeat_usage++],
                           escarp_value(&value[value_usage++], 0x80), 2, 3);
    assert(NULL != parser);

    fp = tmpfile();
    err = escarp_parse(parser, fp, out);
    assert(ESCARP_ERROR_UNEXPECTED_EOF == err);
    fclose(fp);

    fp = tmpfile();
    fwrite((unsigned char[]){0}, 1, 1, fp);
    fseek(fp, 0, SEEK_SET);
    err = escarp_parse(parser, fp, out);
    assert(ESCARP_ERROR_UNEXPECTED_VALUE == err);
    fclose(fp);

    fp = tmpfile();
    fwrite((unsigned char[]){0x80}, 1, 1, fp);
    fseek(fp, 0, SEEK_SET);
    err = escarp_parse(parser, fp, out);
    assert(ESCARP_ERROR_UNEXPECTED_EOF == err);
    fclose(fp);

    fp = tmpfile();
    fwrite((unsigned char[]){0x80, 0x80}, 1, 2, fp);
    fseek(fp, 0, SEEK_SET);
    err = escarp_parse(parser, fp, out);
    assert(ESCARP_SUCCESS == err);
    assert(0x80 == out[0]);
    assert(0x80 == out[1]);
    assert(EOF == out[2]);
    fclose(fp);

    fp = tmpfile();
    fwrite((unsigned char[]){0x80, 0x80, 0x80}, 1, 3, fp);
    fseek(fp, 0, SEEK_SET);
    err = escarp_parse(parser, fp, out);
    assert(ESCARP_SUCCESS == err);
    assert(0x80 == out[0]);
    assert(0x80 == out[1]);
    assert(0x80 == out[2]);
    assert(EOF == out[3]);
    fclose(fp);

    fp = tmpfile();
    fwrite((unsigned char[]){0x80, 0x80, 0x7F}, 1, 3, fp);
    fseek(fp, 0, SEEK_SET);
    err = escarp_parse(parser, fp, out);
    assert(ESCARP_SUCCESS == err);
    assert(0x80 == out[0]);
    assert(0x80 == out[1]);
    assert(EOF == out[2]);
    assert(0x7F == fgetc(fp));
    fclose(fp);

    fp = tmpfile();
    fwrite((unsigned char[]){0x80, 0x80, 0x80, 0x80}, 1, 4, fp);
    fseek(fp, 0, SEEK_SET);
    err = escarp_parse(parser, fp, out);
    assert(0x80 == out[0]);
    assert(0x80 == out[1]);
    assert(0x80 == out[2]);
    assert(EOF == out[3]);
    assert(0x80 == fgetc(fp));
    fclose(fp);

    parser = escarp_repeat(
        &repeat[repeat_usage++],
        escarp_sequence(&sequence[sequence_usage++],
                        escarp_any(&any[any_usage++]),
                        escarp_value(&value[value_usage++], 0x80)),
        2, 3);
    assert(NULL != parser);
    fp = tmpfile();
    fwrite((unsigned char[]){0x0, 0x80, 0x1, 0x80, 0x2, 0x80}, 1, 6, fp);
    fseek(fp, 0, SEEK_SET);
    err = escarp_parse(parser, fp, out);
    assert(ESCARP_SUCCESS == err);
    assert(0x0 == out[0]);
    assert(0x80 == out[1]);
    assert(0x1 == out[2]);
    assert(0x80 == out[3]);
    assert(0x2 == out[4]);
    assert(0x80 == out[5]);
    assert(EOF == out[6]);
    fclose(fp);

    return 0;
}