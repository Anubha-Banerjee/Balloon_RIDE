#include <d3d9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 ) 
#include <iostream>
using namespace std;
#include<stdlib.h>
#include <time.h>
#include <d3dx9.h>
#include <fstream>


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D       = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
LPDIRECT3DVERTEXBUFFER9 g_pVB;// Buffer to hold vertices
LPDIRECT3DTEXTURE9      g_background  ;//backgrnd texture
LPDIRECT3DTEXTURE9      g_girl[8]  ;//girl texture
LPDIRECT3DTEXTURE9      g_bird[8]  ;//bird texture
LPDIRECT3DTEXTURE9      g_bird2[8]  ;//bird texture
LPDIRECT3DTEXTURE9      g_num[10]  ;//number texture
LPDIRECT3DTEXTURE9      g_balloon  ;//balloon texture
LPDIRECT3DVERTEXBUFFER9 foodVB       = NULL; //buffer to hold ballon remaining
LPDIRECT3DTEXTURE9      g_point ;//point texture




wchar_t ani[][50]   =
                {   
					L"J without balloon.bmp",
					L"J with 1 balloon.bmp",
					L"J with balloon.bmp",
					L"J with 3 balloons.bmp",
					L"J with 4balloon.bmp",
					L"J with 5 balloon.bmp",
					L"J with 6 balloon.bmp",

                };
wchar_t anibird[][50]   =
                {
					L"v1.bmp",
					L"v2.bmp",
					L"v3.bmp",
					L"v4.bmp",
					L"v5.bmp",
					L"v6.bmp",
					L"v7.bmp",
                };
wchar_t anibird2[][50]   =
                {
					L"b1.bmp",
					L"b2.bmp",
					L"b3.bmp",
					L"b5.bmp",
					L"b6.bmp",
					L"b7.bmp",
					L"b8.bmp",
					L"b9.bmp",
                };
wchar_t numbers[][50]   =
                {   
					
					L"0.bmp",
					L"1.bmp",
					L"2.bmp",
					L"3.bmp",
					L"4.bmp",
					L"5.bmp",
					L"6.bmp",
					L"7.bmp",
					L"8.bmp",
					L"9.bmp",
                };
int c1=4096,c2=768, a1=0,a2=0,b1=0,b2=768,d1=4096,d2=0;//coordinates of background
int  x1=400,x2=300,yy1=400,yy2=473,z1=506,z2=473,w1=506,w2=300;//coordinates of girl
int  m1=400,m2=300,n1=400,n2=384,o1=470,o2=384,pp1=470,pp2=300;//coordinates of balloon going up
int  i1=800,i2=300,jj1=800,j2=384,k1=870,k2=384,l1=870,l2=300;//coordinates of balloon in air
int dist=2,speed_of_balloon_in_air=3,currtex=2,floatup=0,startfloatingup=0;
int floatdown=0,currtexbird=0,currtexbird2=0;
int startfloatingdown=0;
int p1=1000,p2=300,q1=1000,q2=408,r1=1145,r2=408,s1=1145,s2=300;
int e1=1000,e2=300,f1=1000,f2=388,g1=1088,g2=388,h1=1088,h2=300;
int n=1,vulture=1;
int start_counting_time_since_space=0,count_for_space=13;
int start_counting_time_since_delete=0,count_for_delete=13;
float g=0;//gravity while falling
int skyheight=4,birdpopped=0,skyheight2=1,skyheight3=0,release=0,pressed_delete=0;
int topleft_lifex=0,bottomleft_lifex=0,topright_lifex=50,bottomright_lifex=50;
int topleft_lifey=0,bottomleft_lifey=60,topright_lifey=0,bottomright_lifey=60;
int no_of_balloon=4,nxtlif=0,point=0;
int tlp1x=1065,tlp1y=360,blp1x=1065,blp1y=401,brp1x=1106,brp1y=401,trp1x=1106,trp1y=360;
int r=0;
// A structure for our custom vertex type
struct CUSTOMVERTEX
{
    float x, y, z, rhw; // The transformed position for the vertex
    DWORD color;        // The vertex color

	FLOAT       tu, tv;   // The texture coordinates
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)




//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd )
{
    // Create the D3D object.
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
	/*
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	*/
	
    d3dpp.Windowed = FALSE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = 1024;
	d3dpp.BackBufferHeight = 768;
	
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

     // Create the D3DDevice
     if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
     {
        return E_FAIL;
     }

     // Device state would normally be set here
    g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    for(r=0;r<7;r++)

	{    

	
		 // load the girl
	     D3DXCreateTextureFromFileEx( g_pd3dDevice, (LPCWSTR)	ani[r],     
		 0, 0,1, 0,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT ,D3DX_DEFAULT , D3DX_DEFAULT ,D3DCOLOR_XRGB(77,77,77), 0,0, &g_girl[r]);

		 // load the vulture
	     D3DXCreateTextureFromFileEx( g_pd3dDevice, (LPCWSTR)	anibird[r],     
		 0, 0,1, 0,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT ,D3DX_DEFAULT , D3DX_DEFAULT ,D3DCOLOR_XRGB(77,77,77), 0,0, &g_bird[r]);
		 // load the bird
	     D3DXCreateTextureFromFileEx( g_pd3dDevice, (LPCWSTR)	anibird2[r],     
		 0, 0,1, 0,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT ,D3DX_DEFAULT , D3DX_DEFAULT ,D3DCOLOR_XRGB(77,77,77), 0,0, &g_bird2[r]);

	}
	 for(r=0;r<10;r++)
		 {
			 // load the number textures
			 D3DXCreateTextureFromFileEx( g_pd3dDevice, (LPCWSTR)	numbers[r],     
			 0, 0,1, 0,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT ,D3DX_DEFAULT , D3DX_DEFAULT ,D3DCOLOR_XRGB(77,77,77), 0,0, &g_num[r]);
		 } 
    D3DXCreateTextureFromFileEx( g_pd3dDevice, L"background2.bmp",     
		0, 0,1, 0,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT ,D3DX_DEFAULT , D3DX_DEFAULT ,0,0,0, &g_background); 

	 D3DXCreateTextureFromFileEx( g_pd3dDevice, L"balloon.bmp",     
		0, 0,1, 0,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT ,D3DX_DEFAULT , D3DX_DEFAULT ,D3DCOLOR_XRGB(77,77,77),0,0, &g_balloon); 

	  D3DXCreateTextureFromFileEx( g_pd3dDevice, L"point.bmp",     
		0, 0,1, 0,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT ,D3DX_DEFAULT , D3DX_DEFAULT ,D3DCOLOR_XRGB(77,77,77),0,0, &g_point); 
	 


   // D3DXCreateTextureFromFileEx( g_pd3dDevice, L"J with balloon.bmp",     
   //	0, 0,1, 0,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT ,D3DX_DEFAULT , D3DX_DEFAULT ,D3DCOLOR_XRGB(77,77,77),0,0, &g_girl); 
	

	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InitVB()
// Desc: Creates a vertex buffer and fills it with our vertices. The vertex
//       buffer is basically just a chunk of memory that holds vertices. After
//       creating it, we must Lock()/Unlock() it to fill it. For indices, D3D
//       also uses index buffers. The special thing about vertex and index
//       buffers is that they can be created in device memory, allowing some
//       cards to process them in hardware, resulting in a dramatic
//       performance gain.
//-----------------------------------------------------------------------------




HRESULT InitVB()
{   

    // Create the vertex buffer. Here we are allocating enough memory
    // (from the default pool) to hold all our 3 custom vertices. We also
    // specify the FVF, so the vertex buffer knows what data it contains.
	 g_pd3dDevice->CreateVertexBuffer( 100*sizeof(CUSTOMVERTEX),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB, NULL );
    g_pd3dDevice->CreateVertexBuffer( 300*sizeof(CUSTOMVERTEX),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT,&foodVB , NULL );
    


    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{   
	if( g_pVB != NULL )        
        g_pVB->Release();

	  

    

    if( g_pd3dDevice != NULL ) 
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )       
        g_pD3D->Release();
}

void myDelay() {
		int delay1=2, delay2=2;
		for(int j=0;j<1900000;j++)
		{
			delay1 = 2+2;
			delay2 = delay1+delay2;
		}
}



void release_balloon()
{

  //draw baloon going up
  
  g_pd3dDevice->SetTexture(0, g_balloon);
  g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,30,2);  
   m2=m2-5;
   n2=n2-5;
   o2=o2-5;  
   pp2=pp2-5;
 
   if(n2<0)
     {
	  release=0;
	  pressed_delete=0;
	  m2=300,n2=384,o2=384,pp2=300;
     }


}
void floatupfunction()
{
	if(floatup < 12 && startfloatingup == 1)
	{
		x2	=	x2	- 5;
		yy2 =	yy2	- 5;
		z2  =	z2	- 5;
		w2  =	w2	- 5;
		floatup = floatup + 1;
	}
	else {
		floatup = 0;
		startfloatingup = 0;
	}

}
void floatdownfunction()
{
  if(floatdown<12&&startfloatingdown==1)
			     {
			     x2=x2+5;
			     yy2=yy2+5;
			     z2=z2+5;
                 w2=w2+5;
                 floatdown=floatdown+1;
			     }
  else {
	  floatdown=0;
      startfloatingdown=0;
       }
 //if no balloons are left
   if(currtex==0)
     {
             x2=x2+g;
		     yy2=yy2+g;
		     z2=z2+g;
             w2=w2+g;
			 g=g+0.4;
     }
}
void movebackground()
{

	a1 = a1 - dist;
	b1 = b1 - dist;
	c1 = c1 - dist;
	d1 = d1 - dist;

	if(d1<1024)
	{
     c1=4096,c2=768, a1=0,a2=0,b1=0,b2=768,d1=4096,d2=0;
    }


	i1	=	i1 - speed_of_balloon_in_air;
	jj1	=	jj1 -speed_of_balloon_in_air;
	k1	=	k1 - speed_of_balloon_in_air;
	l1	=	l1 - speed_of_balloon_in_air;
}

void drawfloatingballoon()
{

	i1	=	i1 - speed_of_balloon_in_air;
	jj1	=	jj1 -speed_of_balloon_in_air;
	k1	=	k1 - speed_of_balloon_in_air;
	l1	=	l1 - speed_of_balloon_in_air;

	if(l1<0)
	{
    
	//if(rand()%3==1)
	{
    i1=800,jj1=800,k1=870,l1=870;
    skyheight2=rand()%6;
  if(skyheight2==0)
    {
     i2=60,j2=168,k2=168,l2=60;
     }
  
  if(skyheight2==1)
    {
     i2=120,j2=228,k2=228,l2=120;

    }

  if(skyheight2==2)
    {
     i2=180,j2=288,k2=288,l2=180;

    }

  if(skyheight2==3)
    {
     i2=240,j2=348,k2=348,l2=240;
	 
    }

  if(skyheight2==4)
    {
     i2=300,j2=408,k2=408,l2=300;

    }

  if(skyheight2==5)
    {
     i2=360,j2=468,k2=468,l2=360;
	 
    }


  }
}




}
void drawbird()
{
  srand( (unsigned)time( NULL ) );
  if(currtexbird>6)
   {
    currtexbird=0;
	currtexbird2=0;
   } 
 
  if(vulture==1)
  {
   //y coord of bird can take any of the 7 values as 7 balloons,each top y coord of baloon
   g_pd3dDevice->SetTexture(0, g_bird[currtexbird]);
   g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 12,2 );
   p1=p1-5;
   q1=q1-5;
   r1=r1-5;
   s1=s1-5;
  }
  else
  {
   g_pd3dDevice->SetTexture(0, g_bird2[currtexbird2]);
   g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 18,2 );
   e1=e1-5;
   f1=f1-5;
   g1=g1-5;
   h1=h1-5;
  }	
  if(n%5==0)
  {
  currtexbird=currtexbird++;
  currtexbird2=currtexbird2++;
  }
 
 

  if((s1<0||h1<0)/*&&rand()%2==1*/)
  {
  //60,120,180,240,300,360 p2,e2 can take these values
  skyheight=rand()%6;
  if(skyheight==0)
    {
     p2=60,q2=168,r2=168,s2=60;
	 e2=60,f2=168,g2=168,h2=60;
    }
  
  if(skyheight==1)
    {
     p2=120,q2=228,r2=228,s2=120;
     e2=120,f2=228,g2=228,h2=120;


    }

  if(skyheight==2)
    {
     p2=180,q2=288,r2=288,s2=180;
	 e2=180,f2=288,g2=288,h2=180;
    }

  if(skyheight==3)
    {
     p2=240,q2=348,r2=348,s2=240;
	 e2=240,f2=348,g2=348,h2=240;
    }

  if(skyheight==4)
    {
     p2=300,q2=408,r2=408,s2=300;
	 e2=300,f2=408,g2=408,h2=300;
    }

  if(skyheight==5)
    {
     p2=360,q2=468,r2=468,s2=360;
	 e2=360,f2=468,g2=468,h2=360;
    }

  
  p1=1000,q1=1000,r1=1145,s1=1145;
  e1=1000,f1=1000,g1=1088,h1=1088;
  vulture=rand()%2;
  }
  



}


void checkifbirdpopped()
{  
   if((p2==x2||e2==x2)&&((p1<w1&&s1>w1+50)||(e1<w1&&h1>w1+50)))
   {
		birdpopped=1;
		no_of_balloon=no_of_balloon-1;
   }

   else birdpopped=0;

   if(birdpopped==1 /*&& count_for_delete>12*/)
			{   
                
               //start_counting_time_since_delete=1; //after pressing space texture 1,2,3 balloons should not change quickly
			   //count_for_delete=0;
               if(currtex>=0)
			   {

               currtex=currtex-1;
               startfloatingdown=1;			 
			   }
			   else startfloatingdown=0;
			   
                
			}

}



int abso(int x)
  {
	  if(x<0)
	  {return (-1*x);}

      return x; 
  } 


void life()
{
 CUSTOMVERTEX food[10];
  for(int i=0;i<no_of_balloon;i++)
     {
	
        food[0].x = 0 + nxtlif ;
		food[0].y = 50;
		food[0].z = 1;
		food[0].rhw = 1;
		food[0].color = D3DCOLOR_XRGB(255,255,255);
        food[0].tu=0;
        food[0].tv=1;

		food[1].x = 0 + nxtlif;
		food[1].y = 0;
		food[1].z = 1;
		food[1].rhw = 1;
		food[1].color = D3DCOLOR_XRGB(255,255,255);
        food[1].tu=0;
        food[1].tv=0;



        food[2].x =  50 + nxtlif;
		food[2].y =  50;
		food[2].z = 1;
		food[2].rhw = 1;
		food[2].color = D3DCOLOR_XRGB(255,255,255);
        food[2].tu=1;
        food[2].tv=1;


        food[3].x = 50 + nxtlif ;
		food[3].y =  0;
		food[3].z = 1;
		food[3].rhw = 1;
		food[3].color = D3DCOLOR_XRGB(255,255,255);
        food[3].tu=1;
        food[3].tv=0;


        void *pVertices;
		nxtlif=nxtlif+40;
		g_pd3dDevice->SetStreamSource( 0, foodVB, 0, sizeof(CUSTOMVERTEX) );
	    g_pd3dDevice->SetTexture(0, g_balloon);
        foodVB->Lock( 0, sizeof(food), (void**)&pVertices, 0 );
        memcpy( pVertices, food, sizeof(food) );
        foodVB->Unlock();
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0,2);
  }
  nxtlif=0;
}


void check_if_got_balloon()
{

   if((i2==x2)&&(i1<w1&&i1>w1-10))
   {
	no_of_balloon=no_of_balloon+1;
	i1=-30,jj1=-30,k1=-30,l1=-30;//kick the got balloon out of screen
   }
}


void drawpoint()
{
  
    

   g_pd3dDevice->SetTexture(0, g_point);
   g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,36,2);
   tlp1x=tlp1x-6,blp1x=blp1x-6,brp1x=brp1x-6,trp1x=trp1x-6;
   if(trp1x<0)
     {
		 skyheight3=rand()%6;
		 if(skyheight3==0)
		   {
				tlp1y=60,blp1y=101,brp1y=101,trp1y=60;
           }
  
		 if(skyheight3==1)
		   {
			tlp1y=120,blp1y=161,brp1y=161,trp1y=120;
           }

		if(skyheight3==2)
		  {
			
			tlp1y=180,blp1y=221,brp1y=221,trp1y=180;

		  }  

		if(skyheight3==3)
          {
			 
			 tlp1y=240,blp1y=281,brp1y=281,trp1y=240;
	 
		  }

		if(skyheight3==4)
           {
			 
			 tlp1y=300,blp1y=341,brp1y=341,trp1y=300;
          
           }

		if(skyheight3==5)
		  {
		
			tlp1y=360,blp1y=401,brp1y=401,trp1y=360;
	 
		  }

		tlp1x=1065,blp1x=1065,brp1x=1106,trp1x=1106;
       }

   

}
int i=0,j=0,k=0,arr[3];
void drawscore()
{

g_pd3dDevice->SetTexture(0, g_num[k]);
g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,42,2);
g_pd3dDevice->SetTexture(0, g_num[j]);
g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,48,2);
g_pd3dDevice->SetTexture(0, g_num[i]);
g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,54,2);

}
void setscore()
{

 if(k<=9)
 {
    arr[0]=i;
    arr[1]=j;
    arr[2]=k;
    k=k+1;
 }
 if(point%10==0)
    {
     j=j+1;
     k=0;
    }
  if(point%100==0)
    {
     i=i+1;
     j=0;
     k=0;
   }


drawscore();


}

void check_if_got_point()
{


  if((tlp1y==x2)&&(tlp1x<w1&&tlp1x>w1-10))
   {
	point=point+1;//each gold coin worth 2 points
	setscore();
	tlp1x=-10,blp1x=-10,brp1x=-10,trp1x=-10;//kick got point out of screen
	
   }


}
void check_if_levelend()
{

}


//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
void Render() 
{
    n=n++;//variable to even odd things
	movebackground();

	if(start_counting_time_since_space==1)
	  {
       count_for_space = count_for_space++;
	  }
	if(start_counting_time_since_delete==1)
	  {
       count_for_delete = count_for_delete++;
   	  }

  

    CUSTOMVERTEX vertices[] =
	{
     
        //background
		{ a1, a2,  0, 1.0f, 0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { b1, b2,  0, 1.0f, 0xFFFFFFFF, 0,1},
        { c1, c2,  0, 1.0f, 0xFFFFFFFF, 1,1},

        { a1, a2,  0, 1.0f,  0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { d1, d2, 0, 1.0f,  0xFFFFFFFF, 1,0},
        { c1, c2,  0, 1.0f, 0xFFFFFFFF, 1,1},


		 //girl
		{ x1, x2,  0, 1.0f, 0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { yy1, yy2,  0, 1.0f, 0xFFFFFFFF, 0,1},
        { z1, z2,  0, 1.0f, 0xFFFFFFFF, 1,1},

        { x1, x2,  0, 1.0f,  0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { w1, w2, 0, 1.0f,  0xFFFFFFFF, 1,0},
        { z1, z2,  0, 1.0f, 0xFFFFFFFF, 1,1},

/*
		//imaginary lines
		{ 0, 200,  0, 1.0f, 0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { 0, 205,  0, 1.0f, 0xFFFFFFFF, 0,1},
        { 789, 205,  0, 1.0f, 0xFFFFFFFF, 1,1},

        { 0, 200,  0, 1.0f,  0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { 789,200, 0, 1.0f,  0xFFFFFFFF, 1,0},
        { 789,205,  0, 1.0f, 0xFFFFFFFF, 1,1},


        
		//imaginary lines
		{ 0, 270,  0, 1.0f, 0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { 0, 275,  0, 1.0f, 0xFFFFFFFF, 0,1},
        { 789,275,  0, 1.0f, 0xFFFFFFFF, 1,1},

        { 0, 270,  0, 1.0f,  0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { 789,270, 0, 1.0f,  0xFFFFFFFF, 1,0},
        { 789,275,  0, 1.0f, 0xFFFFFFFF, 1,1},


		//imaginary lines
		{ 0,  340,  0, 1.0f, 0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { 0,  345,  0, 1.0f, 0xFFFFFFFF, 0,1},
        { 789,345,  0, 1.0f, 0xFFFFFFFF, 1,1},

        { 0,  340,  0, 1.0f,  0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { 789,340, 0, 1.0f,  0xFFFFFFFF, 1,0},
        { 789, 345,  0, 1.0f, 0xFFFFFFFF, 1,1},


		
*/
        //bird vulture
		{ p1, p2,  0, 1.0f, 0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { q1, q2,  0, 1.0f, 0xFFFFFFFF, 0,1},
        { r1, r2,  0, 1.0f, 0xFFFFFFFF, 1,1},

        { p1, p2,  0, 1.0f,  0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { s1, s2, 0, 1.0f,  0xFFFFFFFF, 1,0},
        { r1, r2,  0, 1.0f, 0xFFFFFFFF, 1,1},

		
        //bird
		{ e1, e2,  0, 1.0f, 0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { f1, f2,  0, 1.0f, 0xFFFFFFFF, 0,1},
        { g1, g2,  0, 1.0f, 0xFFFFFFFF, 1,1},

        { e1, e2,  0, 1.0f,  0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { h1, h2, 0, 1.0f,  0xFFFFFFFF, 1,0},
        { g1, g2,  0, 1.0f, 0xFFFFFFFF, 1,1},

		 //balloon in air
		{ i1, i2,  0, 1.0f, 0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { jj1, j2,  0, 1.0f, 0xFFFFFFFF, 0,1},
        { k1, k2,  0, 1.0f, 0xFFFFFFFF, 1,1},

        { i1, i2,  0, 1.0f,  0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { l1, l2, 0, 1.0f,  0xFFFFFFFF, 1,0},
        { k1, k2,  0, 1.0f, 0xFFFFFFFF, 1,1},

		 //balloon from girl released
		{ m1, m2,  0, 1.0f, 0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { n1, n2,  0, 1.0f, 0xFFFFFFFF, 0,1},
        { o1, o2,  0, 1.0f, 0xFFFFFFFF, 1,1},

        { m1, m2,  0, 1.0f,  0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { pp1, pp2, 0, 1.0f,  0xFFFFFFFF, 1,0},
        { o1, o2,  0, 1.0f, 0xFFFFFFFF, 1,1},

	/*	//show how many balloon(s)youser has left
		{topleft_lifex,topleft_lifey ,  0, 1.0f, 0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { bottomleft_lifex,bottomleft_lifey ,  0, 1.0f, 0xFFFFFFFF, 0,1},
        { bottomright_lifex, bottomright_lifey ,  0, 1.0f, 0xFFFFFFFF, 1,1},

        { topleft_lifex,topleft_lifey ,  0, 1.0f,  0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { topright_lifex,topright_lifey, 0, 1.0f,  0xFFFFFFFF, 1,0},
        { bottomright_lifex,bottomright_lifey,  0, 1.0f, 0xFFFFFFFF, 1,1},
    */
		
		 //point
		{ tlp1x, tlp1y,  0, 1.0f, 0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { blp1x, blp1y,  0, 1.0f, 0xFFFFFFFF, 0,1},
        { brp1x, brp1y,  0, 1.0f, 0xFFFFFFFF, 1,1},

        { tlp1x, tlp1y,  0, 1.0f,  0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { trp1x, trp1y, 0, 1.0f,  0xFFFFFFFF, 1,0},
        { brp1x, brp1y,  0, 1.0f, 0xFFFFFFFF, 1,1},

			//score ones
		{ 983, 0,  0, 1.0f, 0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { 983, 41,  0, 1.0f, 0xFFFFFFFF, 0,1},
        { 1024,41,  0, 1.0f, 0xFFFFFFFF, 1,1},

        { 983, 0,  0, 1.0f,  0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { 1024,0, 0, 1.0f,  0xFFFFFFFF, 1,0},
        { 1024,41,  0, 1.0f, 0xFFFFFFFF, 1,1},

		//score tens
		{ 945, 0,  0, 1.0f, 0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { 945, 41,  0, 1.0f, 0xFFFFFFFF, 0,1},
        { 986,41,  0, 1.0f, 0xFFFFFFFF, 1,1},

        { 945, 0,  0, 1.0f,  0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { 986,0, 0, 1.0f,  0xFFFFFFFF, 1,0},
        { 986,41,  0, 1.0f, 0xFFFFFFFF, 1,1},

		//score hundreds
		{ 907, 0,  0, 1.0f, 0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { 907, 41,  0, 1.0f, 0xFFFFFFFF, 0,1},
        { 948,41,  0, 1.0f, 0xFFFFFFFF, 1,1},

        { 907, 0,  0, 1.0f,  0xFFFFFFFF, 0,0}, // x, y, z, rhw, color,tex
        { 948,0, 0, 1.0f,  0xFFFFFFFF, 1,0},
        { 948,41,  0, 1.0f, 0xFFFFFFFF, 1,1},

	        
	};


 
     

	
    // Now we fill the vertex buffer. To do this, we need to Lock() the VB to
    // gain access to the vertices. This mechanism is required becuase vertex
    // buffers may be in device (GPU) memory.
   
	VOID* pVertices;

	g_pVB->Lock( 0, sizeof(vertices), (void**)&pVertices, 0 );
 
    memcpy( pVertices, vertices, sizeof(vertices) );

    g_pVB->Unlock();

	


	// Clear the backbuffer to a blue color
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(138,217,255), 1.0f, 0 );

    // Begin the scene
    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
        // Draw the triangles in the vertex buffer. This is broken into a few
        // steps. We are passing the vertices down a "stream", so first we need
        // to specify the source of that stream, which is our vertex buffer. Then
        // we need to let D3D know what vertex shader to use. Full, custom vertex
        // shaders are an advanced topic, but in most cases the vertex shader is
        // just the FVF, so that D3D knows what type of vertices we are dealing
        // with. Finally, we call DrawPrimitive() which does the actual rendering
        // of our geometry (in this case, not just one triangle).
		
		
	    
	        	
        g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof(CUSTOMVERTEX) );

	    //draw background
		g_pd3dDevice->SetTexture(0, g_background);
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0,2);

		//draw girl
		g_pd3dDevice->SetTexture(0, g_girl[currtex]);
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 6,2);

		//draw balloon
		g_pd3dDevice->SetTexture(0, g_balloon);
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 24,2);
		

		//draw imaginary lines
		//g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 12,6); 
 
        check_if_levelend();          
        drawpoint();  
		drawscore();
		check_if_got_point();
        drawfloatingballoon(); 
		drawbird();
		checkifbirdpopped();
		check_if_got_balloon();
        floatdownfunction(); 
	    floatupfunction();
		if(pressed_delete==1)
        release_balloon();
		life();
		
	
		// End the scene
        g_pd3dDevice->EndScene();
    }

    // Present the backbuffer contents to the display
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
			
            Cleanup();
            PostQuitMessage( 0 );
            return 0;

		
		

		case WM_KEYDOWN:

			if(wParam == VK_ESCAPE)
			{
				Cleanup();
				PostQuitMessage( 0 );
				return 0;
			}

			if(wParam == VK_SPACE && count_for_space>12 && currtex!=0&&no_of_balloon>0)// to add and if balloon remaining in the safe
			{  
			   start_counting_time_since_space=1;
			   no_of_balloon=no_of_balloon-1;
			   count_for_space=0;
               if(currtex<6)
			   {
               currtex=currtex+1;
               startfloatingup=1;			 
			   }
			   else startfloatingup=0;
			 
            }
            
			if(wParam == VK_DELETE && count_for_delete>12)// to add and if balloon remaining in the safe
			{  
               pressed_delete=1;
			  
			   m2=x2,n2=m2+84,o2=n2,pp2=x2;
			   start_counting_time_since_delete=1;
			   count_for_delete=0;
			    if(currtex>=0)
			   {
               currtex=currtex-1;
               startfloatingdown=1;			 
			   }
			   else startfloatingdown=0;
            			    
            }
			


			return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
    // Register the window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      L"D3D Tutorial", NULL };
    RegisterClassEx( &wc );

    // Create the application's window
    HWND hWnd = CreateWindow( L"D3D Tutorial", L"D3D Tutorial 02: Vertices",
                              WS_OVERLAPPEDWINDOW, 100, 100, 300, 300,
                              NULL, NULL, wc.hInstance, NULL );
    



     // Initialize Direct3D
    if( SUCCEEDED( InitD3D( hWnd ) ) )
    {
        // Create the vertex buffer
        if( SUCCEEDED( InitVB() ) )
        {
            // Show the window
            ShowWindow( hWnd, SW_SHOWDEFAULT );
            UpdateWindow( hWnd );

            // Enter the message loop
            MSG msg;
            ZeroMemory( &msg, sizeof(msg) );
            while( msg.message!=WM_QUIT )
            {
                if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
                {
                    TranslateMessage( &msg );
                    DispatchMessage( &msg );
                }
				else
				{   
					
 					Render();
					
				}
            }
        }
    }

    UnregisterClass( L"D3D Tutorial", wc.hInstance );
    return 0;
}
