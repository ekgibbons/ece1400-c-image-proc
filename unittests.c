#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "tests/utest.h"

#include "imageio.h"
#include "processing.h"

void round_image(image *y);
double norm(image *x, image *y);
image * read_2d_mtx(char *filename);
void skip_lines(FILE *fp, int n_lines);


UTEST(unittests, read_2d_mtx)
{

    double A_array[] = {
	40, 35, 47,
	2, 42, 62,
	31, -28, 50,
	29, 11, -3
    };

    image *A_sol = malloc(sizeof(*A_sol) +
			  sizeof(A_array));

    A_sol->ny = 4;
    A_sol->nx = 3;
    A_sol->nc = 1;
    
    memcpy(A_sol->data, A_array, sizeof(A_array));
    
    char filename[] = "tests/A_io.mtx";

    image *A = read_2d_mtx(filename);

    ASSERT_EQ(A->ny, 4);
    ASSERT_EQ(A->nx, 3);

    double error = norm(A, A_sol);
	
    ASSERT_LT(error, 10e-6);

    free(A);
    A = NULL;
}

UTEST(imageio, initialize_image)
{
    double A_array[] = {
	1, 2, 3, 4, 5, 6,
	7, 8, 9, 10, 11, 12
    };
    
    image *A = initialize_image(3, 2, 2);
    memcpy(A->data, A_array,
	   sizeof(A_array));
    
    ASSERT_EQ(A->ny,3);
    ASSERT_EQ(A->nx,2);
    ASSERT_EQ(A->nc,2);

    double diff = 0;
    for (int i = 0; i < 12; ++i)
    {
	diff += pow(A->data[i] - A_array[i],2);
    }

    ASSERT_LT(diff, 10e-6);

    free(A);
    A = NULL;
    
}

UTEST(imageio, read_image)
{
    double pix_vals[] = {
	0, 0, 2, 3, 4, 6,
	7, 8, 10, 10, 11, 13,
	9, 10, 12, 13, 14, 16,
	17, 18, 20, 20, 21, 23
    };

    image *im_in = read_image("tests/read_im.jpg");

    ASSERT_EQ(im_in->ny,2);
    ASSERT_EQ(im_in->nx,4);
    ASSERT_EQ(im_in->nc,3);
    
    double diff = 0;
    
    for (int i = 0; i < im_in->ny*im_in->nx*im_in->nc; ++i)
    {
	diff += pow(im_in->data[i] - pix_vals[i], 2);
    }

    ASSERT_LT(diff, 10e-6);
    
    free(im_in);
    im_in = NULL;

}


UTEST(imageio, write_image)
{
    double pix_vals[] = {
	0, 0, 2, 3, 4, 6,
	7, 8, 10, 10, 11, 13,
	9, 10, 12, 13, 14, 16,
	17, 18, 20, 20, 21, 23
    };

    image *im_out = initialize_image(2,4,3);
    memcpy(im_out->data, pix_vals, sizeof(pix_vals));
    
    write_image("write_im.jpg",im_out);

    image *im_in = read_image("write_im.jpg");

    
    double diff = 0;
    
    for (int i = 0; i < im_in->ny*im_in->nx*im_in->nc; ++i)
    {
	diff += pow(im_in->data[i] - pix_vals[i], 2);
    }

    ASSERT_LT(diff, 10e-6);

    free(im_out);
    free(im_in);
    im_out = NULL;
    im_in = NULL;

    int ret = system("rm write_im.jpg");
    (void)ret;

}


UTEST(processing, bw)
{
    int ret = system("python3 tests/python_bw.py");
    ASSERT_EQ(ret, 0);

    image *im_rgb = initialize_image(20,40,3);

    FILE *fp = fopen("color.txt","r");
    for (int i = 0; i < (20*40*3); ++i)
    {
	if (fscanf(fp,"%lf",im_rgb->data+i) == -1)
	{
	    printf("ERROR: fscanf() failed!\n");
	    exit(1);
	}
    }

    fclose(fp);
    
    image *im_bw = initialize_image(20,40,1);

    fp = fopen("bw.txt","r");
    for (int i = 0; i < (20*40*1); ++i)
    {

	if (fscanf(fp,"%lf",im_bw->data+i) == -1)
	{
	    printf("ERROR: fscanf() failed!\n");
	    exit(1);
	}
    }

    fclose(fp);

    image *y = rgb_to_bw(im_rgb);
    double error = norm(y, im_bw);

    ASSERT_LT(error, 10e-6);

    ret = system("rm bw.txt color.txt");
    (void)ret;

    free(im_bw);
    free(im_rgb);
    free(y);
    
    im_bw = NULL;
    im_rgb = NULL;
    y = NULL;

}


UTEST(processing, convolve)
{
    int ret = system("python3 tests/python_convolve.py");
    char filename_x[] = "x_convolve.mtx";
    char filename_h[] = "h_convolve.mtx";
    char filename_y[] = "y_convolve.mtx";

    image *x = read_2d_mtx(filename_x);
    image *h = read_2d_mtx(filename_h);
    image *y_sol = read_2d_mtx(filename_y);
    
    image *y = convolve(x, h);

    double error = norm(y, y_sol);

    ASSERT_LT(error, 10e-6);

    ret = system("rm x_convolve.mtx h_convolve.mtx "
		 "y_convolve.mtx");
    (void)ret;

    free(x);
    free(h);
    free(y_sol);
    free(y);
    
    x = NULL;
    h = NULL;
    y_sol = NULL;
    y = NULL;

}

UTEST(processing, normalize)
{
    int ret = system("python3 tests/python_normalize.py");
    char filename_x[] = "x_normalize.mtx";
    char filename_y[] = "y_normalize.mtx";

    image *x = read_2d_mtx(filename_x);
    image *y_sol = read_2d_mtx(filename_y);
    
    normalize(x, 0, 255);

    double error = norm(x, y_sol);

    ASSERT_LT(error, 10e-6);

    ret = system("rm x_normalize.mtx y_normalize.mtx");
    (void)ret;

    free(x);
    free(y_sol);
    
    x = NULL;
    y_sol = NULL;

}

UTEST(processing, blur)
{
    int ret = system("python3 tests/python_blur.py");
    char filename_x[] = "x_blur.mtx";
    char filename_y[] = "y_blur.mtx";

    image *x = read_2d_mtx(filename_x);
    image *y_sol = read_2d_mtx(filename_y);
    
    image *y = blur(x);

    double error = norm(y, y_sol);

    ASSERT_LT(error, 10e-6);

    ret = system("rm x_blur.mtx y_blur.mtx");
    (void)ret;

    free(x);
    free(y_sol);
    free(y);
    
    x = NULL;
    y_sol = NULL;
    y = NULL;
}


UTEST(processing, sharpen)
{
    int ret = system("python3 tests/python_sharpen.py");
    char filename_x[] = "x_sharp.mtx";
    char filename_y[] = "y_sharp.mtx";
    char filename_strength[] = "strength.txt";

    image *x = read_2d_mtx(filename_x);
    image *y_sol = read_2d_mtx(filename_y);

    double strength;
    FILE *fp = fopen(filename_strength, "r");
    if (fscanf(fp,"%lf",&strength) != 1)
    {
	printf("ERROR: fscanf() failed when "
	       "reading strength\n");
    }

    image *y = sharpen(x, strength);

    double error = norm(y, y_sol);

    ASSERT_LT(error, 10e-6);

    ret = system("rm x_sharp.mtx y_sharp.mtx "
		 "strength.txt");
    (void)ret;

    free(x);
    free(y_sol);
    free(y);
    
    x = NULL;
    y_sol = NULL;
    y = NULL;
}


UTEST(main, usage)
{
    int out = system("./imageprocessing > tmp_1.txt");

    ASSERT_EQ(out, 0);

    FILE *fp = fopen("tmp_2.txt","w");

    fprintf(fp,"Usage:\n    $ ./imageprocessing "
	    "<proc_type> <input_image> <output_image> "
	    "[strength (default=1)]\n");
    fclose(fp);

    out = system("diff -w tmp_1.txt tmp_2.txt");

    ASSERT_EQ(out, 0);

    int ret = system("rm tmp_1.txt tmp_2.txt");
    (void)ret;
    
}


UTEST(main, option)
{
    int out = system("./imageprocessing sharp im1.jpg "
		     "im2.jpg > tmp_1.txt");

    ASSERT_EQ(out, 0);

    FILE *fp = fopen("tmp_2.txt","w");
    fprintf(fp,"processing type must be blur, sharpen, or bw");
    fclose(fp);

    out = system("diff -w tmp_1.txt tmp_2.txt");

    ASSERT_EQ(out, 0);

    out = system("./imageprocessing sharp im1.jpg "
		 "im2.jpg 5 > tmp_1.txt");

    out = system("diff -w tmp_1.txt tmp_2.txt");
	
    ASSERT_EQ(out, 0);
    
    int ret = system("rm tmp_1.txt tmp_2.txt");
    (void)ret;
    
}


UTEST(main, output_bw)
{

    /*                               */
    /* test the output for bw images */
    /*                               */
    
    int out = system("./imageprocessing bw tests/test.jpg "
		     "out_bw.jpg > tmp_1.txt");
    ASSERT_EQ(out, 0);

    image *im_test = read_image("out_bw.jpg");
    image *im_sol = read_image("tests/sol_bw.jpg");

    double error = norm(im_test, im_sol);
    ASSERT_LT(error, 10e-6);

    free(im_test);
    free(im_sol);
    im_test = NULL;
    im_sol = NULL;

    FILE *fp = fopen("tmp_2.txt","w");
    fprintf(fp,"bw processing successful on image size 800x1200\n\n");
    fprintf(fp,"output written to: out_bw.jpg\n");
    fclose(fp);
    
    out = system("diff -w tmp_1.txt tmp_2.txt");
    
    ASSERT_EQ(out, 0);

    int ret = system("rm tmp_1.txt tmp_2.txt out_bw.jpg");
    (void)ret;

}


UTEST(main, output_blur)
{

    /*                                 */
    /* test the output for blur images */
    /*                                 */
    
    int out = system("./imageprocessing blur tests/test.jpg "
		     "out_blur.jpg > tmp_1.txt");
    ASSERT_EQ(out, 0);

    image *im_test = read_image("out_blur.jpg");
    image *im_sol = read_image("tests/sol_blur.jpg");

    double error = norm(im_test, im_sol);
    ASSERT_LT(error, 10e-6);

    free(im_test);
    free(im_sol);
    im_test = NULL;
    im_sol = NULL;

    FILE *fp = fopen("tmp_2.txt","w");
    fprintf(fp,"blur processing successful on image size 800x1200\n\n");
    fprintf(fp,"output written to: out_blur.jpg\n");
    fclose(fp);
    
    out = system("diff -w tmp_1.txt tmp_2.txt");
    
    ASSERT_EQ(out, 0);

    int ret = system("rm tmp_1.txt tmp_2.txt out_blur.jpg");
    (void)ret;

}

UTEST(main, output_sharpen)
{

    /*                                    */
    /* test the output for sharpen images */
    /*                                    */
    
    int out = system("./imageprocessing sharpen tests/test.jpg "
		     "out_sharp.jpg > tmp_1.txt");
    ASSERT_EQ(out, 0);

    image *im_test = read_image("out_sharp.jpg");
    image *im_sol = read_image("tests/sol_sharp.jpg");

    double error = norm(im_test, im_sol);
    ASSERT_LT(error, 10e-6);

    free(im_test);
    free(im_sol);
    im_test = NULL;
    im_sol = NULL;

    FILE *fp = fopen("tmp_2.txt","w");
    fprintf(fp,"sharpen processing successful on image size 800x1200\n\n");
    fprintf(fp,"output written to: out_sharp.jpg\n");
    fclose(fp);
    
    out = system("diff -w tmp_1.txt tmp_2.txt");
    
    ASSERT_EQ(out, 0);

    /* now run the sharpening while specifying the optional strength */
    
    out = system("./imageprocessing sharpen tests/test.jpg "
		 "out_sharp.jpg 2 > tmp_1.txt");
    ASSERT_EQ(out, 0);

    im_test = read_image("out_sharp.jpg");
    im_sol = read_image("tests/sol_sharp_2.jpg");

    error = norm(im_test, im_sol);
    ASSERT_LT(error, 10e-6);

    free(im_test);
    free(im_sol);
    im_test = NULL;
    im_sol = NULL;

    fp = fopen("tmp_3.txt","w");
    fprintf(fp,"sharpen processing successful on image "
	    "size 800x1200\n\n");
    fprintf(fp,"output written to: out_sharp.jpg\n");
    fclose(fp);
    
    out = system("diff -w tmp_1.txt tmp_3.txt");
    
    ASSERT_EQ(out, 0);

    int ret = system("rm tmp_1.txt tmp_2.txt tmp_3.txt "
		     "out_sharp.jpg");
    (void)ret;

}


UTEST_MAIN();

void round_image(image *y)
{
    for (int i = 0; i < (y->nx)*(y->ny); ++i)
    {
	y->data[i] = (double)((int)y->data[i]);
    }
}
    


double norm(image *y, image *x)
{
    double diff = 0;
    for (int i = 0; i < (y->nx)*(y->ny); i++)
    {
	diff += pow(x->data[i] - y->data[i], 2);
    }

    return sqrt(diff);
}



void skip_lines(FILE *fp, int n_lines)
{
    // Skip the first n lines
    for (int i = 0; i < n_lines; ++i)
    {
	if (fscanf(fp, "%*[^\n]\n") == -1)
	{
	    printf("ERROR: fscanf() failed in %s on line %i\n",
		  __FILE__,__LINE__);
	}
    }
}

image * read_2d_mtx(char *filename)
{

    int size_0, size_1;
    
    FILE *fp;
    fp = fopen(filename,"r");

    if (fp == NULL)
    {
	printf("FILE %s not found!\n",filename);
	exit(1);
    }
    
    int n_lines = 2;
    skip_lines(fp, n_lines);
    
    // read in data and find max value
    if (fscanf(fp,"%i %i", &size_0, &size_1) == -1)
    {
	printf("ERROR: fscanf() failed in %s on line %i\n",
	      __FILE__,__LINE__);
    }
    
    image *A = malloc(sizeof(*A) +
		    (size_t)(size_0)*(size_1)*sizeof(double));

    A->ny = size_0;
    A->nx = size_1;
    A->nc = 1;
    
    for (int n = 0; n < A->nx; n++)
    {
	for (int m = 0; m < A->ny ; m++)
	{
	    if ((fscanf(fp,"%lf",A->data+(m*A->nx +n)) != 1) ||
		feof(fp))
	    {
		printf("ERROR: fscanf() failed in %s on line %i\n",
		       __FILE__,__LINE__);

	    }
	}
    }
    fclose(fp);

    return A;
}
