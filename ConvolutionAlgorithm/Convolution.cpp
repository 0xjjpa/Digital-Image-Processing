//
//        >>>Convolution Algorithm in ANSI C<<<
//
//------------------------------------------------------------------------------
//                  Version updated: April 21th, 2012
//                            jjperezaguinaga
//------------------------------------------------------------------------------

//==============================================================================
//------------------------P R O G R A M-----------------------------------------
//==============================================================================

//----------------------------------------------------------------------------//
//         Prototype declarations                                             //
//----------------------------------------------------------------------------//

void heading (void);      /* Displays program heading */
void openfiles (void);    /* Opens input and output files in binary */
void userdefined (void);  /* Here, the user will implement his routine */
void readhdr (void);      /* Read header from input image file */
void addhdr (void);       /* Add header to output image file */
void closefiles (void);   /* Close all files used in program */

//----------------------------------------------------------------------------//
//         Include declarations                                               //
//----------------------------------------------------------------------------//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define DIVIDE_FACTOR 9
//----------------------------------------------------------------------------//
//         Variable declarations                                              //
//----------------------------------------------------------------------------//

int    MRows, NCols;                 /* Number of Rows and columns in image */
FILE   *infptr, *outfptr, *outfptrh; /* Input and output file pointers */
char   infile[40];                   /* Name of input file */
char   outfile[40];                  /* Name of output file */

//----------------------------------------------------------------------------//
//         Main program                                                       //
//----------------------------------------------------------------------------//

int main ()
{
     // Display heading
     heading ();
     // Open Input and Output Image Files
     openfiles ();
     // Read header from image file
     readhdr();
     // Add header to output file
     addhdr();
     // Execute userdefined procedure
     userdefined ();
     // Close all image files
     closefiles ();
     
     printf("\n Bye! Bye!\n");
     system("PAUSE");
     return 1;
} // end main ();

//----------------------------------------------------------------------------//
//         Headings                                                           //
//----------------------------------------------------------------------------//

void heading ()
{
     int i;
     for (i=0; i<16; i++)
       printf("                                    +                       \n");
       printf("                      Convolution Algorithm in ANSI C       \n");
       printf("                          Date: April 21th, 2011            \n");
       printf("                            Author: jjperezaguinaga         \n");
     for (i=0; i<3; i++)
       printf("                                    +                       \n");
       return;
} // end heading ()

//----------------------------------------------------------------------------//
//         Openfiles                                                          //
//----------------------------------------------------------------------------//

void openfiles ()
{
     printf("\n Opening Input and Output image files\n");
     printf(" Enter name of *.pgm INPUT image file (example: lena.pgm) ");
	scanf("%s",infile);
		
     //Check if input file exists
     if ((infptr = fopen(infile, "rb")) == NULL)
     {
       printf(" Cannot open input image file: %s\n",infile);
       printf(" Exiting program...\n");
       system("PAUSE");
       exit(1);
       }
     
     printf(" Enter name of *.pgm OUTPUT image file (example: lenaout.pgm) ");
     scanf("%s",outfile);
     
     //Check if output file was created succesfully
     if ((outfptr = fopen(outfile, "wb")) == NULL)
     {
       printf(" Cannot create output image file: %s\n",outfile);
       printf(" Exiting program...\n");
       system("PAUSE");
       exit(1);
       }
       
     // If this point is reached, file are OK
     printf(" File opened and created OK! \n\n");
     
}  //end openfiles ()

//----------------------------------------------------------------------------//
//         Read Header                                                        //
//----------------------------------------------------------------------------//

void readhdr ()
{
     int   i=0, MaxRGB;
     char  c, buffer[128];
     
     //Read header of PGM file
     printf("\n\n File <%s> Header: \n",infile);
     
     do
     {
       c = fgetc(infptr);
       buffer[i] = c;
       i++; 
     } while (c != '\n');
     
     //Check if file is P5 (pgm) format
     if (buffer[1] == '5')
       printf("\n Input file is pgm, OK\n");
     else
     {
       printf("\n Input file is NOT pgm, Exiting program...\n");
       system("PAUSE");
       exit(0);
     }
     
     //File is pgm, read rest of header
     fscanf(infptr,"%d %d %d",&NCols, &MRows, &MaxRGB);
     c = fgetc(infptr);
     printf("%d ",NCols);
     printf("%d   <---- Width x Height) \n",MRows);
     printf("%d   <---- Max. Grayscale level) \n\n",MaxRGB);
}  // end readhdr ()

//----------------------------------------------------------------------------//
//         Add Header                                                         //
//----------------------------------------------------------------------------//

void addhdr ()
{
     fprintf(outfptr, "P5\n%d %d\n%d\n",NCols,MRows,255);
} //addhdr ()

//----------------------------------------------------------------------------//
//         User defined section                                               //
//----------------------------------------------------------------------------//

void userdefined ()
{
	char container[NCols][MRows];
	char support_container[NCols][MRows];
	char input[10];
	unsigned char c;
	int i = 0, j = 0;
	int mask[3][3];
	
	printf("Please apply an effect to the image (write the number):\n");
	printf("1) Blur image\n");
	printf("2) Vertical Edge Detection Effect\n");
	printf("3) Horizontal Edge Detection Effect\n");
	printf("4) Laplacian Transformation\n");
	scanf("%s", input);
	
	/* MASK DEFINITION */
	const int BLUR_MASK[3][3] = {{1,1,1},{1,1,1},{1,1,1}};
	const int VERTICAL_MASK[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};
	const int HORIZONTAL_MASK[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
	const int LAPLACIAN_MASK[3][3] = {{-1,-1,-1},{-1,8,-1},{-1,-1,-1}};
	/* MASK DEFINITION */
		
	switch (input[0]) 
	{
		case '1':
			for(i=0;i<3;i++) {for(j=0;j<3;j++) {mask[i][j]=BLUR_MASK[i][j];}}
			break;
		case '2':
			for(i=0;i<3;i++) {for(j=0;j<3;j++) {mask[i][j]=VERTICAL_MASK[i][j];}}
			break;
		case '3':
			for(i=0;i<3;i++) {for(j=0;j<3;j++) {mask[i][j]=HORIZONTAL_MASK[i][j];}}
			break;
		case '4':
			for(i=0;i<3;i++) {for(j=0;j<3;j++) {mask[i][j]=LAPLACIAN_MASK[i][j];}}
			break;
		default:
			return;
	}
	
	i = 0;
	j = 0;
	
	while((c = fgetc(infptr)) != (unsigned char)EOF) 
	{
		
		container[i][j] = c;
		j++;
		if(j == NCols) 
		{
			i++;
			j = 0;
		}
	}
	
	
	for(i = 1; i < MRows-1; i++) 
	{
		for(j = 1; j < NCols-1; j++) 
		{
			support_container[i][j] = (
				 mask[0][0] * container[i-1][j-1] +
				 mask[0][1] * container[i][j-1] +
				 mask[0][2] * container[i+1][j-1] +
				 mask[1][0] * container[i-1][j] +
				 mask[1][1] * container[i][j] +
				 mask[1][2] * container[i+1][j] +
				 mask[2][0] * container[i-1][j+1] +
				 mask[2][1] * container[i][j+1] +
				 mask[2][2] * container[i+1][j+1] ) / DIVIDE_FACTOR;
		}
	} 
	
	// BORDERS //
	for(i = 0; i < NCols; i++) 
	{
		support_container[0][i] = container[0][i];
	}
	for(i = 1; i < MRows; i++) 
	{
		support_container[i][0] = container[i][0];
	}
	for(i = 1; i < MRows; i++) 
	{
		support_container[i][NCols-1] = container[i][NCols-1];
	}
	for(i = 1; i < NCols-1; i++) 
	{
		support_container[MRows-1][i] = container[MRows-1][i];
	}
	// BORDERS //
	
	//PRINT FILE
	for(i = 0; i < MRows; i++) {
		for(j = 0; j < NCols; j++) {
			fprintf(outfptr, "%c",support_container[i][j]);
		}
	}
}  // end userdefined ()

//----------------------------------------------------------------------------//
//         Close Files                                                        //
//----------------------------------------------------------------------------//

void closefiles ()
{
     fclose(infptr);
     fclose(outfptr);
}  // end closefiles ()
