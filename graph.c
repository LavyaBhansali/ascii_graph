/*
2 jan 2026, 17:00
to make a graph, wherein you input points, and graph is made where you can see those points. currently this 
graph is going to consist only the first quadrant. initial idea, make a large 2x2 array, and change elements of 
that array to show the graph. 

- completed
1)to make the quadrant somewhat symmetric, i made a array of size 35x70, and filled half the column with 
alternating spaces, to even it out.

2)added details to quadrant, such as axes, origin and values of axes, denoted by '*', at multiples of 5
3)graph rendering

- to do, inputting values, plotter, scaling, showing the scale, and points printed

logging out 2 jan 2026, 19:30
*/

/*
3 jan 26, 23:30

-completed
1)implemented value inputer,
2)added plotter

logging out 4jan 00:48
*/

/*
4th jan 26, 14:27
-To implement scaler, but i have designing questions, what if the imputs have high variance?,
for eg 1000,1000 and 2-3 same values, in that case since there is scaleing, it is possible that to to represnt
on scaled graph, a few near valued points get displayed on the same coordinate

-i have accepted the fact that some values will be plotted on the same cell
-first approach thought- take the coodinate with max sqrt(x2+Y2) value and plot it the furthest, i.e on 33, 33,
but a problem with that is, say you have 65, 80 and 80, 65, what point will you plot, this and few more small
implementation issue were observed.
-thought of a better approach, so now what ihave decided to do is that take all x values and all the y values,
and take the ,maximum of that, and according to max x and y, i will scale.
- but i have two options with this one, because if calculate max of x and y separately,
there will be different scale of the axes, and another idea it that i take max of max (x and y),
and then put that scale both the axes according to that so that I have equal scale for that.
- decided that i will give user the option for what he wants.
chatgpts courtesy for naming the options;-
“Fit to Grid” vs “True Scale”
“Independent Scaling” vs “Uniform Scaling”
“Maximize Space” vs “Preserve Shape”
i'll go with second one. 
-To do:
 implement side functions such as roundoff, 
 value acceptor, which will error handle to accept appropriate value scale values 

to not scale if scale less than 33.

 cases where a single cell has multiple plots, a diff symbol, and below output of the number of plots on
 that cell

multiple sessions throughout totalling to not more that 3hrs of active work.
*/

/*
5th jan 26 10:49
implemented scale only if required,(if scaling is less than 33), scaling wont happen
to do:-
value accepting function, no. of plots in cell, safety clamping to avoid IEEE-754 floating point behavior
errors, beatification of code, if applicable at some places by using functions for repeated things.
for no. of plots in a cell, i have two options, mutate the original array to count or create a copy or
second array (scaled), so that I preserve original array, useful if I ever expand this project, having
original inputs could be helpful

two sessions, totalling to 1hr, not much done today
*/

/*11 feb 26, 09:30
added error handling for value accepting

finishing of code, just making minor changes to make it ready to upload to github eg:-

used define for row and column, to make it easier to change the size of the graph if needed, also to make the
code more readable, and maintainable, also added comments to explain the code, and to make it more
understandable, also added a note on axis collision due to scaling, and scale display vs plotted position,
to explain the behavior of the graph when scaling is applied, and to communicate the limitations of the graph
due to finite resolution and rounding.
*/

//FOR THE USER
/* A coordinate plotting program, 1) enter number of points you want to plot, 
2) enter the coordinates (note only positive coordinates, since it has only first quadrant)
3) select a scale, if you want same scale for x and y azis then select uniform scale,

special features, it scales axes according to the inputs, if it has large inputs,
it will scale to fit all points in,
due to scaling, some points may be plotted on the same cell, to handle that,
the program shows how many plots are made in a single cell.

HOW TO USE:
1) compile the code using a C compiler that supports C17 standard, for example:
gcc graphs.c -o graphs && ./graphs

2) enter the number of points you want to plot, should be a positive integer.

3) enter the coordinates of the points, should be two positive integers separated by space, for example:
10 20
438 32

4) select the scaling option, enter 1 for independent scaling, or 2 for uniform scaling.

5) the graph will be displayed, along with the scale information and the number of points plotted
on each cell if there are multiple points on the same cell.

Thankyou.
-xerotheta
*/

#include<stdio.h>
#define row 35  
#define column 70

int roundoff(float num);

int main()
{
    //To accept the number of points to plot, should be positive int, error handled

    printf("Enter the number of points to plot: ");         
    int plots;
    rep:
    if(scanf("%d",&plots) !=1 || plots<=0)      //error handling for number of plots, should be positive int
    {
        printf("Invalid Input, should be positive int.\n");
        while(getchar()!='\n');
        goto rep;
    }
    char quadrant[row][column];     //initial canvas, to be updated to show the graph, and then printed


    /* to fill the quadrant with spaces and dots, spaces for even columns, and dots for odd columns, 
    to make it somewhat symmetric, since characters are more wide than they are tall, so to make it somewhat 
    symmetric, i have filled half the column with spaces, and half with dots, this is just a design choice, 
    and can be changed if needed*/

    for(int i=0; i<row; i++)
    {
        for (int j=0; j<column; j++)
        {
            if(j%2==0)
            {
                quadrant[i][j]=' ';
                continue;
            }
            quadrant[i][j]='.';
        }
    }

    // to create axes

    for (int j=1; j<row-1; j++)
    {
        quadrant[j][3]='|';
        quadrant[row-2][j*2+1]='_';
    }
    //few more details changes

    quadrant[0][3]='^';
    quadrant[row-2][3]='+';
    quadrant[row-2][column-1]='>';
    quadrant[row-1][1]='o';
    quadrant[row-2][1]='*';       
    quadrant[row-1][3]='*';

     //see if these line can be avoided by using this in loop
     //not feasible


    //to print values of axes
    for (int i=1; i<row; i++)
    {

        if (i%5==0)
        {
            quadrant[i-2][1]='*';
            quadrant[row-1][i*2+3]='*';
        }
    }

    // to take input of coordinates, and find the max of x and y to decide the scale
    int arrx[plots], arry[plots], xmax=0, ymax=0;   

    for(int i=0;i<plots;i++)
    {
        /*error handling for coordinates, should be two positive int separated by space, also to 
        handle the problem of input with space, and to enter nth coordinate, for that i have used goto 
        statement, goto is generally discouraged, but here it is used deliberately to retry a single input point
        without introducing additional state variables or nested loops. This keeps the control flow explicit
        and localized.
        separate
        */
        printf("\nEnter The coordinates: ");
        rep1:
        if(scanf("%d%d",&arrx[i],&arry[i])!=2 || arrx[i]<0 || arry[i]<0)
        {
            printf("Invalid input, enter two positive int separated by space then press enter.\n");
            while(getchar()!='\n');
            goto rep1;
        }                        
        //fix this, eneter nth coordinate, and inputs with space problem, try inputting 3-4points only with space 
        //is this fixed? //
        
        
        if(arrx[i]>xmax)
        {
            xmax=arrx[i];
        }
        if (arry[i]>ymax)
        {
            ymax=arry[i];
        }
    }

    // to select the scale, and then scale the points according to the selected scale
    printf("\n\nSelect scaling option:\n");
    printf("\t1) Independant scaling\n");
    printf("\t2) Uniform scaling\n");
    printf("Enter 1 or 2: ");
    int scale;
    //error handling for scale, should be 1 or 2
    rep2:
    if (scanf("%d", &scale) != 1 || !(scale == 1 || scale == 2))
    {
        printf("Invalid input, input should be '1' or '2' only");
        while(getchar()!='\n');
        goto rep2;
    }    

    // to print the points plotted
    printf("\n\n\nGRAPH INFO:\n");
    printf("\tPoints plotted: \n");
    for(int i=0;i<plots;i++)
    {
        printf("\t\t%d: (%d, %d),\n",i+1,arrx[i],arry[i]);
    }

    int sx[plots], sy[plots]; //scaled coordinated

    switch(scale)
    {
        //INDEPENDANT SCALING
        case 1:
        {
            float xscale, yscale;

            //to check if scaling is required, if not then scale will be 1
            xscale=(xmax>row-2)? (row-2.0)/xmax:1;
            yscale=(ymax>row-2)? (row-2.0)/ymax:1;

            /*to scale the points according to the selected scale, and then plot them on the quadrant, 
            also to update the arrx and arry with the scaled values, so that we can use them later to check if 
            two points are plotted on the same cell or not, and also to print the scaled coordinates of the points 
            */

            for(int i=0;i<plots;i++)
            {
                sx[i]=roundoff(arrx[i]*xscale);
                if(sx[i]>row-2)
                {
                    sx[i]=row-2;
                }

                sy[i]=roundoff(arry[i]*yscale);
                if(sy[i]>row-2)
                {
                    sy[i]=row-2;
                }

                quadrant[row-2-sy[i]][3+2*sx[i]]='X'; 
                /* THE ABOVE TWO LINES TO UPDATE ARRX AND ARRY ARE NOT NECESSARY, THE OPERATION CAN BE DIRECTLY
                USED IN THIS STATEMENT, BUT I HAVE NOT done that, TO MAKE the CODE MORE UNDERSTANDABLE, ALSO
                UPDATING ARRX AND ARRY WILL BE HELPFUL TO SEE IF TWO PLOTS HAVE SAME CELLS  */
            }

            // to print the scale information
            printf("\nScaling format: Independant scaling\n");
            printf("\nScale:\n\t\t One X axis unit '.' = %d",roundoff(1/xscale));
            printf("\t\t One Y axis unit '.' = %d",roundoff(1/yscale));
            printf("\n\t\tFive X axis unit '*' = %d",roundoff(5/xscale));
            printf("\t\tFive Y axis unit '*' = %d\n",roundoff(5/yscale));
            break;
        }

        //UNIFORM SCALING
        case 2:
        {
            float uniscale;
            
            /*to check if scaling is required, if not then scale will be 1, and to find the uniform scale
            according to the maximum of x and y */
            uniscale=(xmax>ymax)? ((xmax>row-2)? (row-2.0)/xmax:1):((ymax>row-2)?(row-2.0)/ymax:1);

            //again scaling the points and plotting them on the quadrant
            for(int i=0;i<plots;i++)
            {
                sx[i]=roundoff(arrx[i]*uniscale);
                if(sx[i]>row-2)
                {
                    sx[i]=row-2;
                }
                sy[i]=roundoff(arry[i]*uniscale);    
                if(sy[i]>row-2)
                {
                    sy[i]=row-2;
                }
                quadrant[row-2-sy[i]][3+2*sx[i]]='X';
            }

            // to print the scale information
            printf("\nScaling format: Uniform scaling\n");
            printf("\nScale:\n\t\t One unit '.' = %d",roundoff(1/uniscale));
            printf("\t\t Five unit '*' = %d\n", roundoff(5/uniscale));
            break;
        }
    }


    /* to check if two or more points are plotted on the same cell, and to print the number of points 
    plotted on that cell, and also to change the symbol of that cell to '#' to indicate that there are 
    multiple points plotted on that cell, also to print the coordinates of that cell and the number of 
    points plotted on that cell*/

    for(int i=0; i<plots;i++)
    {
        int counter=0;
        for(int j=i+1;j<plots;j++)
        {
            if(sx[i]==sx[j] && sy[i]==sy[j] && sx[i]!=-1)
            {
                counter++;
                sx[j]=-1;
            }
        }
        if(counter)
        {
            printf("For '#' :\n");
            printf("At cell (%d, %d) plots = %d\n",sx[i],sy[i],counter+1);
            if(scale==1)
            {
                quadrant[row-2-sy[i]][3+2*sx[i]]='#';
            }
            else
            {
                quadrant[row-2-sy[i]][3+2*sx[i]]='#';
            }
        }
    }

    // to print the quadrant
    for(int i=0;i<row;i++)
    {
        for(int j=0; j<column; j++)
        {
            printf("%c",quadrant[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}

// roundoff function
int roundoff(float num)
{
    int rem;
    num*=100;
    rem=(int)num%100;
    num/=100;
    if(rem>=50)
    {
        num++;
        return num;
    }
    else
    {
        return num;
    }
}

/* value acceptor function
 values to be accepted at
 1) no of plots, should be whole number
 2) points, should be whole number
 3) scale, should be 1 or 2
 4) 
*/









/*
NOTE ON AXIS COLLISION DUE TO SCALING:

When scaling large coordinate ranges into a finite grid, small values
(e.g., (2,1)) may map to the same cell as the axes after rounding.
This is an expected consequence of finite resolution and is not treated
as a bug.

The axes should be interpreted as "bins" for near-zero values at the
chosen scale, not as exact zero-only lines.

Scale information is displayed to communicate this approximation.
*/


/*
NOTE ON SCALE DISPLAY VS PLOTTED POSITION:

The scale values shown below the graph are rounded for readability.
Internally, the plotting uses the actual (fractional) scale values.

Because of this, a point may appear slightly earlier or later than what
a simple multiplication using the displayed scale suggests.
*/