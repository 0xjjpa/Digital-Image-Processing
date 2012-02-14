//+++++++++++++++++++++++++++++++<M D I P>++++++++++++++++++++++++++++++++++++++
//
//     >>>MASK DIGITAL IMAGE PROCESSING Program<<<
//
//        This program reads an input color image in *.bpm format
//        and converts it into a grayscale image in *.pgm format
//        using the following equation:
//            Gray(x,y) = 1/3 (Red(x,y) + Green(x,y) + Blue(x,y)
//        for every every pixel located at (x,y) in the image
//
//------------------------------------------------------------------------------
//                  Version updated: February 10, 2012
//                            jjperezaguinaga
//------------------------------------------------------------------------------

//==============================================================================
//------------------------P R O G R A M-----------------------------------------
//==============================================================================
//
//  main();
//     heading();               display program heading
//     openfiles();             opens input & output image files
//     readhdr();               reads input image file header
//     closefiles();            closes input and processed output image files
//------------------------------------------------------------------------------
void     heading(void);
void     openfiles(void);
void     convert(void);
void     readhdr(void);
void     closefiles();
void     display(void);
//------------------------------------------------------------------------------
# include <stdio.h>
# include <math.h>
# include <stdlib.h>
# include <string.h>
//------------------------------------------------------------------------------
int      width, height;                     //indices for rows and cols
FILE     *infptr, *outfptr;    //input/output file pointers
FILE     *outfptrh_r, *outfptrh_g, *outfptrh_b;    //input/output file pointers
char     infile[40];
char     static  outfile[40];              //names of input/output image files

//------------------------------------------------------------------------------
//-----------------------------MAIN---------------------------------------------
//------------------------------------------------------------------------------
main()
{
    //--------------( Display Heading with information )------------------------
    heading();
    //---------------(Open Input & Output Image Files)--------------------------
    openfiles(); 
    //---------------------(Convert to grayscale)-------------------------------
    convert();
    //------------------(Close Any Open Image Files)----------------------------
    closefiles();
    //--------------------------------------------------------------------------
    printf("\n Bye! Bye!\n");
    system("PAUSE");
    return (1);
} //end main()

//------------------------------------------------------------------------------
//----------------------------HEADING-------------------------------------------
//------------------------------------------------------------------------------
void heading ()
{
     int i;
     for (i=0; i<16; i++)
       printf("                                    +                       \n");
       printf("                           BMP to Grayscale in C		       \n"); 
       printf("                          Date: February 10, 2012            \n");
       printf("                    Author: jjperezaguinaga - 888059        \n");
     for (i=0; i<3; i++)
       printf("                                    +                       \n");
       return;
} // end heading ()

//------------------------------------------------------------------------------
//--------------------------OPENFILES-------------------------------------------
//------------------------------------------------------------------------------
void openfiles(void)
{
  printf("\n                 OPEN an image file\n");
  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf(" Enter name of *.bmp INPUT image file: ? ");
  scanf("%s",&infile);
  printf(" Enter name of *.pgm OUTPUT image file: ? ");
  scanf("%s",&outfile);
    
  if ((infptr = fopen(infile, "rb")) == NULL)
  { printf(" Can NOT open input image file: <%s>\n",infile);
    printf(" Exiting program..... "); system("PAUSE"); exit(1);
  }
  else  printf(" Input file <%s> opened sucessfully\n\n",infile);
  
  if ((outfptr = fopen(outfile,"wb")) == NULL)
  { printf(" Can NOT open output image file <%s>\n\n",outfile);
    printf(" Exiting program....."); system("PAUSE"); exit(1);
  }
  else printf(" Output file <%s> is opened sucessfully\n\n",outfile);
  
  if ((outfptrh_r = fopen("histogram_red.txt","wb")) == NULL)
  { printf(" Can NOT open output red histogram file <%s>\n\n",outfile);
    printf(" Exiting program....."); system("PAUSE"); exit(1);
  }  
  
  if ((outfptrh_g = fopen("histogram_green.txt","wb")) == NULL)
  { printf(" Can NOT open output green histogram file <%s>\n\n",outfile);
    printf(" Exiting program....."); system("PAUSE"); exit(1);
  }
  
  if ((outfptrh_b = fopen("histogram_blue.txt","wb")) == NULL)
  { printf(" Can NOT open output green histogram file <%s>\n\n",outfile);
    printf(" Exiting program....."); system("PAUSE"); exit(1);
  }
  
  readhdr();
  return;
}//end openfiles()

//------------------------------------------------------------------------------
//------------------------------READHDR-----------------------------------------
//------------------------------------------------------------------------------
void readhdr()
{
  //-----------------------[Read BPM File Header]-------------------------------
  printf("\n\n File <%s> Header Bytes:\n",infile);
  printf("------------------------------------------------------------\n");
 
  int first, temp = 0;
  void * trash = malloc(18);
    
	//READ BMP HEADER
	fread(&first, 2, 1, infptr);
	
	if((char)first != 'B')
	{
	   printf(" Image is in WRONG format!! Quitting.........\n\n");
       system("PAUSE");
       exit(0);
	}
	else
	{
		printf("FORMAT: BMP CORRECT\n");
	}
	
	fread(trash, 16, 1, infptr);
	fread(&width, 4, 1, infptr);
	fread(&height, 4, 1, infptr);
	fread(trash, 16, 1, infptr);
	fread(trash, 12, 1, infptr);
		
	printf("WIDTH: %d\n", width);
	printf("HEIGHT: %d\n", height);
}//end readhdr()

//------------------------------------------------------------------------------
//-------------------------conversion and historgram----------------------------
//------------------------------------------------------------------------------
void convert()
{
  unsigned char R,G,B,Gray;
  unsigned int histcount_r[256];
  unsigned int histcount_g[256];
  unsigned int histcount_b[256];
  char string[10];
  
  //Initialize histogram count to 0
  for (int i=0;i<256;i++)
  {
   histcount_r[i] = 0;
   histcount_g[i] = 0;
   histcount_b[i] = 0;
  }
    
  //Add *.pgm Header
  fprintf(outfptr,"P5\n%d %d\n255\n", width, height);
  
  //Perform conversions
  R = fgetc(infptr);
  do{    
     G = fgetc(infptr);
     B = fgetc(infptr);
     Gray =(R + G + B) / 3;
     //Increment histogram
     histcount_r[R]++;
     histcount_g[G]++;
     histcount_b[B]++;
     fputc(Gray,outfptr);
     //Read next pixel to check if we have an End of File
     R = fgetc(infptr);
  } while (!feof(infptr));
  
  //Convert the histogram values from integers to string
  for (int i=0;i<256;i++){
	fprintf(outfptrh_r,"%d: %d\n", i, histcount_r[i]);
	fprintf(outfptrh_g,"%d: %d\n", i, histcount_g[i]);
	fprintf(outfptrh_b,"%d: %d\n", i, histcount_b[i]);
  }
}

//------------------------------------------------------------------------------
//----------------------------CLOSEFILES----------------------------------------
//------------------------------------------------------------------------------
void closefiles()
{ //------------------------(Close Files)---------------------------------------
  fclose(infptr);
  fclose(outfptr);
  fclose(outfptrh_r);
  fclose(outfptrh_g);
  fclose(outfptrh_b);
  return;
} //end closefiles()
//------------------------------------------------------------------------------
