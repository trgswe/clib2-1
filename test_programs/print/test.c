/*
 * $Id: test.c,v 1.6 2005-08-26 12:21:51 clib2devs Exp $
 *
 */

#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>

void __attribute__ ((constructor)) constructor_test1(void);
void __attribute__ ((constructor)) constructor_test2(void);
void __attribute__ ((constructor)) constructor_test3(void);
void __attribute__ ((destructor)) destructor_test1(void);
void __attribute__ ((destructor)) destructor_test2(void);
void __attribute__ ((destructor)) destructor_test3(void);

void __attribute__ ((constructor))
constructor_test1(void) {
    fprintf(stderr, "constructor #1 called\n");
}

void __attribute__ ((constructor))
constructor_test2(void) {
    fprintf(stderr, "constructor #2 called\n");
}

void __attribute__ ((destructor))
destructor_test1(void) {
    fprintf(stderr, "destructor #1 called\n");
}

void __attribute__ ((destructor))
destructor_test2(void) {
    fprintf(stderr, "destructor #2 called\n");
}

void __attribute__ ((destructor))
destructor_test3(void) {
    /*DebugPrintF("__iob = 0x%08lx\n",__iob);*/

    fprintf(stderr, "destructor #3 called\n");
}

int foo = 3;
int bar = 9;

int
main(int argc, char **argv) {
    struct timeval tv;
    time_t now;
    struct tm tm;
    int i, j, k;
    long n, r;
    char time_buffer[100];

    free(malloc(4));

    for (i = 0; i < argc; i++)
        printf("%2d) \"%s\"\n", i, argv[i]);

    printf("div %d mod %d\n", foo / 2, bar % 4);

    time(&now);
    printf("%s", ctime(&now));

    tm = (*localtime(&now));
    strftime(time_buffer, sizeof(time_buffer), "%c", &tm);
    printf("local time = %s\n", time_buffer);

    tm = (*gmtime(&now));
    strftime(time_buffer, sizeof(time_buffer), "%c", &tm);
    printf("gmt = %s\n", time_buffer);

    tm = (*gmtime(&now));
    strftime(time_buffer, sizeof(time_buffer), "%c %U %W %j", &tm);
    printf("%s\n", time_buffer);

    tm.tm_year = 105;
    tm.tm_mon = 0;
    tm.tm_mday = 25;
    tm.tm_hour = 12;
    tm.tm_min = 3;
    tm.tm_sec = 53;
    tm.tm_isdst = -1;

    now = mktime(&tm);

    tm = (*localtime(&now));
    strftime(time_buffer, sizeof(time_buffer), "%c", &tm);
    printf("2005-01-25 12:03:53 -> local time = %s\n", time_buffer);

    tm = (*gmtime(&now));
    strftime(time_buffer, sizeof(time_buffer), "%c", &tm);
    printf("2005-01-25 12:03:53 -> gmt = %s\n", time_buffer);

    {
        const double pi = 3.14159265358979323846;
        const double ten = 10.0;
        const double quarter = 0.25;
        const double thousand = 1000.0;
        const double foo = 4 * atan((double) 1);
        float f1;
        double d1;

        printf("pi=%3.1f (float)\n", pi);
        printf("pi=%.21e (exponential)\n", pi);
        printf("pi=%g (float/exponential)\n", pi);

        printf("ten=%f (float)\n", ten);
        printf("ten=%.21e (exponential)\n", ten);
        printf("ten=%g (float/exponential)\n", ten);

        printf("thousand=%f (float)\n", thousand);
        printf("thousand=%.21e (exponential)\n", thousand);
        printf("thousand=%g (float/exponential)\n", thousand);

        printf("quarter=%f (float)\n", quarter);
        printf("quarter=%.21e (exponential)\n", quarter);
        printf("quarter=%g (float/exponential)\n", quarter);

        printf("foo=%f (float)\n", foo);
        printf("foo=%.21e (exponential)\n", foo);
        printf("foo=%g (float/exponential)\n", foo);

        printf("32 bit float = %f\n", 4294967295.0);
        printf("32+1 bit float = %f\n", -4294967295.0);
        printf("big float on the edge = %f\n", 4294967296.0);
        printf("big float = %f\n", 429496729654321.0);
        printf("small float = %f\n", -429496729654321.0);

        f1 = d1 = 9;

        r = sscanf("13.24 1.324", "%f %lf", &f1, &d1);
        printf("r = %ld, f1 = %f, d1 = %f\n", r, f1, d1);
    }

#ifndef NDEBUG
    {
        char *allocation;

        allocation = malloc(4);
        if (allocation != NULL) {
            strcpy(allocation, "....FOO");
            strcpy(allocation - 3, "bar");
        }
    }
#endif /* NDEBUG */

    printf("hex 0x%08x\n", 1);
    printf("hex 0x%08x\n", 1);
    printf("hex 0x%08x\n", 2);
    printf("big int %d\n", 0x80000000L);
    printf("converted big int %d\n", atoi("-2147483648"));

    r = sscanf("1324", "%lx", &n);
    printf("r = %ld, n = %ld\n", r, n);

    r = sscanf("1234567890", "%4d%3d%3d", &i, &j, &k);
    printf("r = %ld, i = %d, j = %d, k = %d\n", r, i, j, k);

    {
        const char *arg = "100x100";
        float xres = 0, yres = 0;

        printf("%d: ", sscanf(arg, "%fx%f", &xres, &yres));
        printf("%.02fx%.02f\n", xres, yres);
    }

    gettimeofday(&tv, 0);
    printf("time = %d.%06d\n", tv.tv_sec, tv.tv_usec);

    if (argc > 1) {
        DIR *dir;

        dir = opendir(argv[1]);
        if (dir != NULL) {
            struct dirent *d;
            struct stat st;

            chdir(argv[1]);

            while ((d = readdir(dir)) != NULL) {
                if (stat(d->d_name, &st) == 0)
                    printf("%s%s\n", d->d_name, S_ISDIR(st.st_mode) ? " (dir)" : "");
            }

            closedir(dir);
        }
    }

    return (0);
}
