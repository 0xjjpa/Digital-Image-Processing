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

# include <stdio.h>
# include <math.h>
# include <stdlib.h>
# include <string.h>

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

main ()
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
     return(1);
} // end main ();

//----------------------------------------------------------------------------//
//         Headings                                                           //
//----------------------------------------------------------------------------//

void heading ()
{
     int i;
     for (i=0; i<16; i++)
       printf("                                    +                       \n");
       printf("                      Digital Image Processing in C         \n");
       printf("                          Date: February 6, 2012            \n");
       printf("                    Author: jjperezaguinaga - 888059        \n");
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
     scanf("%s",&infile);
     
     //Check if input file exists
     if ((infptr = fopen(infile, "rb")) == NULL)
     {
       printf(" Cannot open input image file: %s\n",infile);
       printf(" Exiting program...\n");
       system("PAUSE");
       exit(1);
       }
     
     printf(" Enter name of *.pgm OUTPUT image file (example: lenaout.pgm) ");
     scanf("%s",&outfile);
     
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
	int c;
	int choice = 0;
	int i = 0;
	int j = 0;
	int buffer[NCols*MRows];
	
		printf("What do you want to perform on your out image?\n");
		printf("1.- Light it up\n2.- Dark it up\n3.- Rotate 90 degrees\n4.- Reflect horizontal\n5.- Reflect vertical\n6.- Exit");
		scanf("%d", &choice); 
		
		switch(choice)
		{
			case 1:
				for (; i< NCols*MRows; i++)
				{
					c = (int) fgetc(infptr);
					c = (c + 30 > MaxRGB) ? MaxRGB : c + 30;
					fprintf(outfptr, "%c",c);
				}
			break;
				
			case 2:
				for (; i< NCols*MRows; i++)
				{
					c = (int) fgetc(infptr);
					c = (c - 30 < 0) ? 0 : c - 30;
					fprintf(outfptr, "%c",c);
				}
			break;
			
			case 3:
				
				for (; i< NCols*MRows; i++)
				{
						c = (int) fgetc(infptr);
						buffer[i] = c;
				}
				
				
				for (i = 0; i < MRows; i++)
				{
					for (j = NCols; j > 0; j--)
					{
						fprintf(outfptr, "%c",buffer[i+(NCols*j)]);
					}
				}
				
			break;
			
			case 4:
				for (; i< NCols*MRows; i++)
				{
						c = (int) fgetc(infptr);
						buffer[i] = c;
				}
				
				
				for (i = 0; i < MRows; i++)
				{
					for (j = NCols; j > 0; j--)
					{
						fprintf(outfptr, "%c",buffer[(NCols*i)+j]);
					}
				}
			break;
			
			case 5:
				for (; i< NCols*MRows; i++)
				{
						c = (int) fgetc(infptr);
						buffer[i] = c;
				}
				
				for (i = MRows; i > 0; i--)
				{
					for (j = 0; j < NCols; j++)
					{
						fprintf(outfptr, "%c",buffer[(NCols*i)+j]);
					}
				}
			break;
			
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
