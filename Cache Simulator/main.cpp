//Author: Mahmoud Shaheen
//Cache Simulator
//Compiled using GNU GCC Compiler
//Computer Organization Course "Prof: Hazem I. Shehata"

/*
Mapping Functions:

1-Direct Mapping
2-Full_Associative Mapping
3-Set_Associative Mapping

*/

/*
Replacement Algorithms:

1-FIFO
2-LFU
3-LRU
4-Random

*/

/*
Write-Hit Policies:

1-Write_Trough
2-Write_Back

*/

/*
Write-Miss Policies:

1-Write_Allocate
2-No_Write_Allocate

*/

/*
Cache Array Segmentation:

1-Cache[x][0]: Valid indicator
2-Cache[x][1]: Tag
3-Cache[x][2]: Replacement Algorithm Counter
4-Cache[x][3]:  Dirty-bit

*/

#include <iostream>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <bitset>

using namespace std;

int replacementalgorithm,writehit,writemiss;
float totalhit=0,totalmiss=0;//For Calculating Hit-Ratio
int it=0;//for FIFO algorithm

class cache
{
    int memorysize,blocksize,cachesize;
    int cache[65536][4]; //64-K Lines
    int l;//#of Lines in Cache
    int ss;//#of Sets in Cache
    int w;//#of Word-bits
    int s;//#of Block-bits
    int r;//#of Line-bits
    int d;//#of Set-bits
    int t;//#of Tag-bits
    int k;//#of Lines per Set "K-Ways"
    int tag;
    bitset<32> tagbits;
    bitset<32> blockno;
    int lineno,setno;
    int found=0;
    int temp; //temporary variable for l
    int tempadd; //temporary address
    int temp2;


public:

//Set Values
    void direct_set(int ch,int mem, int blk)
    {
        memorysize=mem;
        blocksize=blk;
        cachesize=ch;
        l=cachesize/blocksize;
        temp=l; // use it for loop as l changes
        for (int i=0 ; i<temp ; i++) //to initialize the cache with zeros
        {
            for (int j=0 ; j<4 ; j++)
            {
                cache[i][j]=0;
            }
        }

        w=log2(blocksize);
        s=log2(memorysize/blocksize);
        r=log2(cachesize/blocksize);
        t=s-r;
        cout << "\n Address Size= " << log2(memorysize) <<endl;
        cout << "\n W= "<<w<< "\n S= "<<s<< "\n R= "<<r<< "\n T= "<<t <<endl;
    }

    void full_set(int ch,int mem, int blk)
    {
        memorysize=mem;
        blocksize=blk;
        cachesize=ch;
        l=cachesize/blocksize;
        temp=l; // use it for loop as l changes
        for (int i=0 ; i<temp ; i++) //to initialize the cache with zeros
        {
            for (int j=0 ; j<4 ; j++)
            {
                cache[i][j]=0;
            }
        }

        w=log2(blocksize);
        s=log2(memorysize/blocksize);
        t=s;
        cout << "\n Address Size= " << log2(memorysize) <<endl;
        cout << "\n W= "<<w<< "\n S= "<<s<< "\n T= "<<t <<endl;
    }

    void set_set(int ch,int mem, int blk, int kk)
    {
        memorysize=mem;
        blocksize=blk;
        cachesize=ch;
        l=cachesize/blocksize;
        k=kk;
        temp=l; // use it for loop as l changes
        for (int i=0 ; i<temp ; i++) //to initialize the cache with zeros
        {
            for (int j=0 ; j<4 ; j++)
            {
                cache[i][j]=0;
            }
        }

        w=log2(blocksize);
        s=log2(memorysize/blocksize);
        ss=cachesize/(k*blocksize);
        d=log2(ss);
        t=s-d;
        cout << "\n Address Size= " << log2(memorysize) <<endl;
        cout << "\n W= "<<w<< "\n S= "<<s<< "\n D= "<<d<< "\n T= "<<t <<endl;
    }



//Read Operation
    void direct_read (int location)
    {
        tagbits=location;
        tagbits.to_string();
        tagbits=(tagbits>>w);
        tagbits=(tagbits>>r);
        tag=tagbits.to_ulong();

        l=cachesize/blocksize;
        blockno=location;
        blockno.to_string();
        blockno=(blockno>>w);
        lineno=blockno.to_ulong() % l;
        cout << "\n Tag: " << tag << endl;
        cout << "\n Block: " << blockno.to_ulong() << endl;
        cout << "\n Line: " << lineno <<endl;
        if( cache[lineno][0]!=NULL && tag == cache[lineno][1] )//block is in cache
        {
                cout << "\n Read Hit \n\n No-Line Allocate" << endl;
                totalhit++;
        }
        else //block is not in cache
        {
            cout << "\n Read miss" << endl;
            totalmiss++;
            if (writehit==2 && cache[lineno][3]==1)//write back and dirty bit check
                cout <<"\n Write Back" << endl;
            cout << "\n Line Allocate"<< endl;
            cache[lineno][3]=0; //reset dirty bit
            cache[lineno][0]=1; //set valid bit
            cache[lineno][1]=tag; //copy the new tag to cache

        }
    }

    void full_read (int location)
    {
        tagbits=location;
        tagbits.to_string();
        tagbits=(tagbits>>w);
        tag=tagbits.to_ulong();

        l=cachesize/blocksize;
        cout << "\n Tag: " << tag << endl;
        temp=l;
        for (int i=0 ; i < temp ; i++)
        {
            if( cache[i][0]!=NULL && tag == cache[i][1] )//block is in cache
            {
                cout << "\n Read Hit \n\n No-Line Allocate" << endl;
                totalhit++;
                if( replacementalgorithm == 2)//RA bit update LFU
                    cache[i][2]++;
                if( replacementalgorithm == 3 )//RA bit update LRU
                {
                    temp2=cache[0][2];
                    for (int j=0 ; j<l ;j++)//to get the RA bit of LRU block
                    {
                        if(cache[j][2]>temp2)
                            temp2=cache[j][2];
                    }
                    temp2++;
                    cache[i][2]=temp2;//reset R.A bit
                }
                found=1;
                cout << "\n Line: " << i <<endl;
                break;
            }
        }

        if(found==0) //block is not in cache
        {
            cout << "\n Read miss" << endl;
            totalmiss++;
            switch(replacementalgorithm)
            {
                case 1://FIFO
                {
                    temp=cache[0][2];
                    tempadd=0;
                    for (int i=0 ; i<l ; i++ )
                    {
                        if (cache[i][2]<temp)//R.A. bit is smaller than current value
                        {
                            temp=cache[i][2];
                            tempadd=i;
                        }
                    }
                    if (writehit==2 && cache[tempadd][3]==1)//write back and dirty bit check
                        cout <<"\n Write Back" << endl;
                    cout << "\n Line Allocate"<< endl;
                    cout << "\n Line: " << tempadd <<endl;
                    cache[tempadd][3]=0; //reset dirty bit
                    cache[tempadd][2]=it;//reset R.A bit
                    cache[tempadd][0]=1; //set valid bit
                    cache[tempadd][1]=tag; //copy the new tag to cache
                    break;
                }
                case 2://LFU
                {
                    temp=cache[0][2];
                    tempadd=0;
                    for (int i=0 ; i<l ; i++ )
                    {
                        if (cache[i][2]<temp)//R.A. bit is smaller than current value
                        {
                            temp=cache[i][2];
                            tempadd=i;
                        }
                    }
                    if (writehit==2 && cache[tempadd][3]==1)//write back and dirty bit check
                        cout <<"\n Write Back" << endl;
                    cout << "\n Line Allocate"<< endl;
                    cout << "\n Line: " << tempadd <<endl;
                    cache[tempadd][3]=0; //reset dirty bit
                    cache[tempadd][2]=0;//reset R.A bit
                    cache[tempadd][0]=1; //set valid bit
                    cache[tempadd][1]=tag; //copy the new tag to cache
                    break;
                }
                case 3://LRU
                {
                    temp=cache[0][2];
                    tempadd=0;
                    for (int i=0 ; i<l ; i++ )
                    {
                        if (cache[i][2]<temp)//R.A. bit is smaller than current value
                        {
                            temp=cache[i][2];
                            tempadd=i;
                        }
                    }
                    if (writehit==2 && cache[tempadd][3]==1)//write back and dirty bit check
                        cout <<"\n Write Back" << endl;
                    cout << "\n Line Allocate"<< endl;
                    cout << "\n Line: " << tempadd <<endl;
                    cache[tempadd][3]=0; //reset dirty bit
                    temp=cache[0][2];
                    for (int i=0 ; i<l ;i++)//to get the RA bit of LRU block
                    {
                        if(cache[i][2]>temp)
                            temp=cache[i][2];
                    }
                    temp++;
                    cache[tempadd][2]=temp;//reset R.A bit
                    cache[tempadd][0]=1; //set valid bit
                    cache[tempadd][1]=tag; //copy the new tag to cache
                    break;
                }
                case 4://Random
                {
                    tempadd=rand() % l;
                    if (cache[tempadd][0]==1 && writehit==2 && cache[tempadd][3]==1)//write back and dirty bit check
                        cout <<"\n Write Back" << endl;
                    cout << "\n Line Allocate"<< endl;
                    cout << "\n Line: " << tempadd <<endl;
                    cache[tempadd][3]=0; //reset dirty bit
                    cache[tempadd][2]=0;//reset R.A bit
                    cache[tempadd][0]=1; //set valid bit
                    cache[tempadd][1]=tag; //copy the new tag to cache
                    break;
                }
            }

        }

        found=0;
    }

    void set_read (int location)
    {
        int first,last;//First & Last Elements in the set
        tagbits=location;
        tagbits.to_string();
        tagbits=(tagbits>>w);
        tagbits=(tagbits>>d);
        tag=tagbits.to_ulong();


        l=cachesize/blocksize;
        ss= cachesize/(k*blocksize);
        blockno=location;
        blockno.to_string();
        blockno=(blockno>>w);
        setno=blockno.to_ulong() % ss;
        first=k*setno;
        last=first+(k-1);
        cout << "\n Tag: " << tag << endl;
        cout << "\n Set: " << setno << endl;
        temp= last;
        for (int i= first ; i <= temp ; i++)//search in the set
        {
            if( cache[i][0]!=NULL && tag == cache[i][1] )//block is in cache
            {
                cout << "\n Read Hit \n\n No-Line Allocate" << endl;
                totalhit++;
                if( replacementalgorithm == 2)//RA bit update LFU
                    cache[i][2]++;
                if( replacementalgorithm == 3 )//RA bit update LRU
                {
                    temp2=cache[first][2];
                    for (int j=first ; j<=temp ;j++)//to get the RA bit of LRU block
                    {
                        if(cache[j][2]>temp2)
                            temp2=cache[j][2];
                    }
                    temp2++;
                    cache[i][2]=temp2;//reset R.A bit
                }
                found=1;
                cout << "\n Line: " << i <<endl;
                break;
            }
        }

        if(found==0) //block is not in cache
        {
            cout << "\n Read miss" << endl;
            totalmiss++;
            switch(replacementalgorithm)
            {
                case 1://FIFO
                {
                    temp=cache[first][2];
                    tempadd=first;
                    for (int i=first ; i<=last ; i++ )
                    {
                        if (cache[i][2]<temp)//R.A. bit is smaller than current value
                        {
                            temp=cache[i][2];
                            tempadd=i;
                        }
                    }
                    if (writehit==2 && cache[tempadd][3]==1)//write back and dirty bit check
                        cout <<"\n Write Back" << endl;
                    cout << "\n Line Allocate"<< endl;
                    cout << "\n Line: " << tempadd <<endl;
                    cache[tempadd][3]=0; //reset dirty bit
                    cache[tempadd][2]=it;//reset R.A bit
                    cache[tempadd][0]=1; //set valid bit
                    cache[tempadd][1]=tag; //copy the new tag to cache
                    break;
                }
                case 2://LFU
                {
                    temp=cache[first][2];
                    tempadd=first;
                    for (int i=first ; i<=last ; i++ )
                    {
                        if (cache[i][2]<temp)//R.A. bit is smaller than current value
                        {
                            temp=cache[i][2];
                            tempadd=i;
                        }
                    }
                    if (writehit==2 && cache[tempadd][3]==1)//write back and dirty bit check
                        cout <<"\n Write Back" << endl;
                    cout << "\n Line Allocate"<< endl;
                    cout << "\n Line: " << tempadd <<endl;
                    cache[tempadd][3]=0; //reset dirty bit
                    cache[tempadd][2]=0;//reset R.A bit
                    cache[tempadd][0]=1; //set valid bit
                    cache[tempadd][1]=tag; //copy the new tag to cache
                    break;
                }
                case 3://LRU
                {
                    temp=cache[first][2];
                    tempadd=first;
                    for (int i=first ; i<=last ; i++ )
                    {
                        if (cache[i][2]<temp)//R.A. bit is smaller than current value
                        {
                            temp=cache[i][2];
                            tempadd=i;
                        }
                    }
                    if (writehit==2 && cache[tempadd][3]==1)//write back and dirty bit check
                        cout <<"\n Write Back" << endl;
                    cout << "\n Line Allocate"<< endl;
                    cout << "\n Line: " << tempadd <<endl;
                    cache[tempadd][3]=0; //reset dirty bit
                    temp=cache[first][2];
                    for (int i=first ; i<=last ;i++)//to get the RA bit of LRU block
                    {
                        if(cache[i][2]>temp)
                            temp=cache[i][2];
                    }
                    temp++;
                    cache[tempadd][2]=temp;//reset R.A bit
                    cache[tempadd][0]=1; //set valid bit
                    cache[tempadd][1]=tag; //copy the new tag to cache
                    break;
                }
                case 4://Random
                {
                    tempadd=rand() % k + first;
                    if (cache[tempadd][0]==1 && writehit==2 && cache[tempadd][3]==1)//write back and dirty bit check
                        cout <<"\n Write Back" << endl;
                    cout << "\n Line Allocate"<< endl;
                    cout << "\n Line: " << tempadd <<endl;
                    cache[tempadd][3]=0; //reset dirty bit
                    cache[tempadd][2]=0;//reset R.A bit
                    cache[tempadd][0]=1; //set valid bit
                    cache[tempadd][1]=tag; //copy the new tag to cache
                    break;
                }
            }

        }

        found=0;
    }




//Write Operation
    void direct_write (int location)
    {
        tagbits=location;
        tagbits.to_string();
        tagbits=(tagbits>>w);
        tagbits=(tagbits>>r);
        tag=tagbits.to_ulong();

        l=cachesize/blocksize;
        blockno=location;
        blockno.to_string();
        blockno=(blockno>>w);
        lineno=blockno.to_ulong() % l;
        cout << "\n Tag: " << tag << endl;
        cout << "\n Block: " << blockno.to_ulong() << endl;
        cout << "\n Line: " << lineno <<endl;
        if( cache[lineno][0]!=NULL && tag == cache[lineno][1] )//block is in cache
        {
            cout << "\n Write Hit" << endl;
            totalhit++;
            if (writehit==1) //write through chosen
                cout << "\n Write Through" << endl;
            else //write back chosen
            {
                cache[lineno][3]=1;//change dirty bit
            }
        }
        else //block is not in cache
        {
            cout << "\n Write Miss " << endl;
            totalmiss++;
            if (writemiss==1) //write allocate chosen
            {
                cout << "\n Line Allocate" << endl;
                if (writehit==2 && cache[lineno][3]==1)//write back and dirty bit check
                    cout <<"\n Write Back" << endl;
                cache[lineno][0] =1;
                cache[lineno][1]=tag;
                cache[lineno][3]=0;//reset dirty bit
            }
            else //No-write allocate chosen
                cout << "\n No-Line Allocate " << endl;
        }
    }


    void full_write (int location)
    {
        tagbits=location;
        tagbits.to_string();
        tagbits=(tagbits>>w);
        tag=tagbits.to_ulong();

        l=cachesize/blocksize;

        cout << "\n Tag: " << tag << endl;
        temp=l;
        for (int i=0 ; i < temp ; i++)
        {
            if( cache[i][0]!=NULL && tag == cache[i][1] )//block is in cache
            {
                cout << "\n Write Hit \n\n No-Line Allocate" << endl;
                totalhit++;
                if( replacementalgorithm == 2)//RA bit update LFU
                    cache[i][2]++;
                if( replacementalgorithm == 3 )//RA bit update LRU
                {
                    temp2=cache[0][2];
                    for (int j=0 ; j<l ;j++)//to get the RA bit of LRU block
                    {
                        if(cache[j][2]>temp2)
                            temp2=cache[j][2];
                    }
                    temp2++;
                    cache[i][2]=temp2;//reset R.A bit
                    //write hit policy
                    if (writehit==1) //write through chosen
                        cout << "\n Write Through" << endl;
                    else //write back chosen
                    {
                        cache[i][3]=1;//change dirty bit
                    }
                }
                found=1;
                cout << "\n Line: " << i <<endl;
                break;
            }
        }

        if(found==0) //block is not in cache
        {
            cout << "\n Write miss" << endl;
            totalmiss++;
            if (writemiss==1) //write allocate chosen
            {
                switch(replacementalgorithm)
                {
                    case 1://FIFO
                    {
                        temp=cache[0][2];
                        tempadd=0;
                        for (int i=0 ; i<l ; i++ )
                        {
                            if (cache[i][2]<temp)//R.A. bit is smaller than current value
                            {
                                temp=cache[i][2];
                                tempadd=i;
                            }
                        }
                        if (writehit==2 && cache[tempadd][3]==1)//write back and dirty bit check
                            cout <<"\n Write Back" << endl;
                        cout << "\n Line Allocate"<< endl;
                        cout << "\n Line: " << tempadd <<endl;
                        cache[tempadd][3]=0; //reset dirty bit
                        cache[tempadd][2]=it;//reset R.A bit
                        cache[tempadd][0]=1; //set valid bit
                        cache[tempadd][1]=tag; //copy the new tag to cache
                        break;
                    }
                    case 2://LFU
                    {
                        temp=cache[0][2];
                        tempadd=0;
                        for (int i=0 ; i<l ; i++ )
                        {
                            if (cache[i][2]<temp)//R.A. bit is smaller than current value
                            {
                                temp=cache[i][2];
                                tempadd=i;
                            }
                        }
                        if (writehit==2 && cache[tempadd][3]==1)//write back and dirty bit check
                            cout <<"\n Write Back" << endl;
                        cout << "\n Line Allocate"<< endl;
                        cout << "\n Line: " << tempadd <<endl;
                        cache[tempadd][3]=0; //reset dirty bit
                        cache[tempadd][2]=0;//reset R.A bit
                        cache[tempadd][0]=1; //set valid bit
                        cache[tempadd][1]=tag; //copy the new tag to cache
                        break;
                    }
                    case 3://LRU
                    {
                        temp=cache[0][2];
                        tempadd=0;
                        for (int i=0 ; i<l ; i++ )
                        {
                            if (cache[i][2]<temp)//R.A. bit is smaller than current value
                            {
                                temp=cache[i][2];
                                tempadd=i;
                            }
                        }
                        if (writehit==2 && cache[tempadd][3]==1)//write back and dirty bit check
                            cout <<"\n Write Back" << endl;
                        cout << "\n Line Allocate"<< endl;
                        cout << "\n Line: " << tempadd <<endl;
                        cache[tempadd][3]=0; //reset dirty bit
                        temp=cache[0][2];
                        for (int i=0 ; i<l ;i++)//to get the RA bit of LRU block
                        {
                            if(cache[i][2]>temp)
                                temp=cache[i][2];
                        }
                        temp++;
                        cache[tempadd][2]=temp;//reset R.A bit
                        cache[tempadd][0]=1; //set valid bit
                        cache[tempadd][1]=tag; //copy the new tag to cache
                        break;
                    }
                    case 4://Random
                    {
                        tempadd=rand() % l;
                        if (cache[tempadd][0]==1 && writehit==2 && cache[tempadd][3]==1)//write back and dirty bit check
                            cout <<"\n Write Back" << endl;
                        cout << "\n Line Allocate"<< endl;
                        cout << "\n Line: " << tempadd <<endl;
                        cache[tempadd][3]=0; //reset dirty bit
                        cache[tempadd][2]=0;//reset R.A bit
                        cache[tempadd][0]=1; //set valid bit
                        cache[tempadd][1]=tag; //copy the new tag to cache
                        break;
                    }
                }

            }
            else //No-write allocate chosen
                cout << "\n No-Line Allocate " << endl;
        }
        found=0;
    }

    void set_write (int location)
    {
        int first,last;//First & Last Elements in the set
        tagbits=location;
        tagbits.to_string();
        tagbits=(tagbits>>w);
        tagbits=(tagbits>>d);
        tag=tagbits.to_ulong();


        l=cachesize/blocksize;
        ss= cachesize/(k*blocksize);
        blockno=location;
        blockno.to_string();
        blockno=(blockno>>w);
        setno=blockno.to_ulong() % ss;
        first=k*setno;
        last=first+(k-1);
        cout << "\n Tag: " << tag << endl;
        cout << "\n Set: " << setno << endl;
        temp= last;
        for (int i=first ; i <= last ; i++)
        {
            if( cache[i][0]!=NULL && tag == cache[i][1] )//block is in cache
            {
                cout << "\n Write Hit \n\n No-Line Allocate" << endl;
                totalhit++;
                if( replacementalgorithm == 2)//RA bit update LFU
                    cache[i][2]++;
                if( replacementalgorithm == 3 )//RA bit update LRU
                {
                    temp2=cache[first][2];
                    for (int j=first ; j<=last ;j++)//to get the RA bit of LRU block
                    {
                        if(cache[j][2]>temp2)
                            temp2=cache[j][2];
                    }
                    temp2++;
                    cache[i][2]=temp2;//reset R.A bit
                    //write hit policy
                    if (writehit==1) //write through chosen
                        cout << "\n Write Through" << endl;
                    else //write back chosen
                    {
                        cache[i][3]=1;//change dirty bit
                    }
                }
                found=1;
                cout << "\n Line: " << i <<endl;
                break;
            }
        }

        if(found==0) //block is not in cache
        {
            cout << "\n Write miss" << endl;
            totalmiss++;
            if (writemiss==1) //write allocate chosen
            {
                switch(replacementalgorithm)
                {
                    case 1://FIFO
                    {
                        temp=cache[first][2];
                        tempadd=first;
                        for (int i=first ; i<=last ; i++ )
                        {
                            if (cache[i][2]<temp)//R.A. bit is smaller than current value
                            {
                                temp=cache[i][2];
                                tempadd=i;
                            }
                        }
                        if (writehit==2 && cache[tempadd][3]==1)//write back and dirty bit check
                            cout <<"\n Write Back" << endl;
                        cout << "\n Line Allocate"<< endl;
                        cout << "\n Line: " << tempadd <<endl;
                        cache[tempadd][3]=0; //reset dirty bit
                        cache[tempadd][2]=it;//reset R.A bit
                        cache[tempadd][0]=1; //set valid bit
                        cache[tempadd][1]=tag; //copy the new tag to cache
                        break;
                    }
                    case 2://LFU
                    {
                        temp=cache[first][2];
                        tempadd=first;
                        for (int i=first ; i<=last ; i++ )
                        {
                            if (cache[i][2]<temp)//R.A. bit is smaller than current value
                            {
                                temp=cache[i][2];
                                tempadd=i;
                            }
                        }
                        if (writehit==2 && cache[tempadd][3]==1)//write back and dirty bit check
                            cout <<"\n Write Back" << endl;
                        cout << "\n Line Allocate"<< endl;
                        cout << "\n Line: " << tempadd <<endl;
                        cache[tempadd][3]=0; //reset dirty bit
                        cache[tempadd][2]=0;//reset R.A bit
                        cache[tempadd][0]=1; //set valid bit
                        cache[tempadd][1]=tag; //copy the new tag to cache
                        break;
                    }
                    case 3://LRU
                    {
                        temp=cache[first][2];
                        tempadd=first;
                        for (int i=first ; i<=last ; i++ )
                        {
                            if (cache[i][2]<temp)//R.A. bit is smaller than current value
                            {
                                temp=cache[i][2];
                                tempadd=i;
                            }
                        }
                        if (writehit==2 && cache[tempadd][3]==1)//write back and dirty bit check
                            cout <<"\n Write Back" << endl;
                        cout << "\n Line Allocate"<< endl;
                        cout << "\n Line: " << tempadd <<endl;
                        cache[tempadd][3]=0; //reset dirty bit
                        temp=cache[first][2];
                        for (int i=first ; i<=last ;i++)//to get the RA bit of LRU block
                        {
                            if(cache[i][2]>temp)
                                temp=cache[i][2];
                        }
                        temp++;
                        cache[tempadd][2]=temp;//reset R.A bit
                        cache[tempadd][0]=1; //set valid bit
                        cache[tempadd][1]=tag; //copy the new tag to cache
                        break;
                    }
                    case 4://Random
                    {
                        tempadd=rand() % k + first ;
                        if (cache[tempadd][0]==1 && writehit==2 && cache[tempadd][3]==1)//write back and dirty bit check
                            cout <<"\n Write Back" << endl;
                        cout << "\n Line Allocate"<< endl;
                        cout << "\n Line: " << tempadd <<endl;
                        cache[tempadd][3]=0; //reset dirty bit
                        cache[tempadd][2]=0;//reset R.A bit
                        cache[tempadd][0]=1; //set valid bit
                        cache[tempadd][1]=tag; //copy the new tag to cache
                        break;
                    }
                }

            }
            else //No-write allocate chosen
                cout << "\n No-Line Allocate " << endl;
        }
        found=0;
    }


};


int main()
{
    int cachesize,memorysize,blocksize;
    int mappingfunction;
    int location;
    char operation;
    cache cache;
    int hitratio=0;
    int k;

    system( "color 0A" ); //To adjust Colors for background"0" and Font"A"
    cout << "\n\t\t     Welcome To Physical Cache Simulator ! \n\n";

//Entering Memory Parameters
    cout << "\n Enter Memory Size in bytes \n" << endl;
    cin >> memorysize;
    cout << "\n Enter Cache Size in bytes \"#Lines < 64-k\" \n" << endl;
    cin >> cachesize;
    cout << "\n Enter Block Size in bytes \n" << endl;
    cin >> blocksize;
    if(memorysize==0 || blocksize==0 || cachesize==0)
    {
        cout << "\n Memory/Cache/block Size equal to zero!" << endl;
        exit(EXIT_FAILURE);
    }
//Wrong Parameters handlers
    if (cachesize > memorysize)
    {
        cout << "\n Cache Size is Bigger than Memory size!" << endl;
        exit(EXIT_FAILURE);
    }
    if (blocksize > memorysize || blocksize > cachesize)
    {
        cout << "\n Block Size is too big!" << endl;
        exit(EXIT_FAILURE);
    }
    if (cachesize/blocksize > 65536)//Maximum Lines Number
    {
        cout << "\n #Lines is too big!" << endl;
        exit(EXIT_FAILURE);
    }

//Mapping Function and Policies
    cout << "\n Enter Mapping Function \n 1-Direct \n 2-Full_Associative \n 3-Set_Associative \n";
    cin >> mappingfunction;
    if ( mappingfunction !=1 && mappingfunction !=2 && mappingfunction !=3 )
        {
            cout << "\n Unknown Mapping Function!" << endl;
            exit(EXIT_FAILURE);
        }
    if(mappingfunction==3)
    {
        cout << "\n Enter K \n";
        cin >> k ;
        if (k==0 || k==1 || k>=cachesize )
        {
            cout << "\n Wrong Value of K! " <<endl;
            exit(EXIT_FAILURE);
        }
        if(k==(cachesize/blocksize))
        {
            cout<<"\n K = #of Lines In Cache --> Fully Associative!" << endl;
            mappingfunction=2;
        }
    }

    if (mappingfunction!=1)
    {
        cout<<"\n Choose Replacement Algorithm \n 1-FIFO \n 2-LFU \n 3-LRU \n 4-Random \n";
        cin >> replacementalgorithm;
        if (replacementalgorithm !=1 && replacementalgorithm !=2 && replacementalgorithm !=3 && replacementalgorithm !=4)
        {
            cout << "\n Unknown Replacement Algorithm" << endl;
            exit(EXIT_FAILURE);
        }
    }
    cout <<"\n Choose Write-Hit policy \n 1-Write-Through \n 2-Write-Back \n";
    cin >> writehit;
    if (writehit !=1 && writehit !=2 )
        {
            cout << "\n Unknown Write-Hit Policy!" << endl;
            exit(EXIT_FAILURE);
        }
    cout << "\n Choose Write-Miss policy \n 1-Write-Allocate \n 2-No-Write-Allocate \n";
    cin >> writemiss;
    if (writemiss !=1 && writemiss !=2 )
    {
        cout << "\n Unknown Write-Miss Policy" << endl;
        exit(EXIT_FAILURE);
    }


    switch (mappingfunction)
    {
        case 1: //Direct Mapped
        {
            cache.direct_set(cachesize,memorysize,blocksize);
            break;
        }
        case 2: //Full Associative Mapped
        {
            cache.full_set(cachesize,memorysize,blocksize);
            break;

        }
        case 3: //Set Associative Mapped
        {
            cache.set_set(cachesize,memorysize,blocksize,k);
            break;
        }
        default://Errors
        {
            cout << "\n Error: Unknown Mapping Function \n";
            exit (EXIT_FAILURE);
        }
    }

//Read/Write/Exit
    while (true)
    {
        it++;
        cout << "\n Enter R/W Instruction followed by memory location in decimal or \"E\" to Exit \n";
        cin >> operation;
        if (operation== 'R' || operation== 'W')
            cin >> location ;

        switch (operation)
        {
            case 'R': //For Reads
            {
                if (location > memorysize-1)
                {
                    cout << "\n Invalid Location"<<endl;
                    break;
                }
                if(mappingfunction==1)
                    cache.direct_read(location);
                if(mappingfunction==2)
                    cache.full_read(location);
                if(mappingfunction==3)
                    cache.set_read(location);
                break;
            }

            case 'W': //For Writes
            {
                if (location > memorysize-1)
                {
                    cout << "\n Invalid Location"<<endl;
                    break;
                }
                if(mappingfunction==1)
                    cache.direct_write(location);
                if(mappingfunction==2)
                    cache.full_write(location);
                if(mappingfunction==3)
                    cache.set_write(location);
                break;
            }

            case 'E'://For Exit
            {
                if (totalhit>0 || totalmiss>0)
                    hitratio=100*(totalhit/(totalhit+totalmiss));
                cout << "\n Hit Ratio = " << hitratio << " % " << endl;
                exit(EXIT_SUCCESS);
                break;
            }

            default://For Wrong Inputs
            {
                cout << "\n Error: Unknown Operation  \n";
                break;
            }
        }
    }

    return 0;
}
