void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  outHsl(31,240,50);
    delay(2000);

}
int outHsl(float h, float s, float l) {

  float  r = 0,g = 0, b = 0;
 
  float c,x;
  int hh;
  
  if( h>=360 ) h=359;
  if( s>100 ) s=100;
  if( l>100 ) l=100;
  
  s/=100;
  l/=100;
  c = (1-fabs(2*l-1))*s;
  hh = h/60;
  x = c*(1-fabs (hh%2-1));

if( hh>=0 && hh<1 )
      {
        r = c;
        g = x;
      }
      else if( hh>=1 && hh<2 )
      {
        r = x;
        g = c;
      }
      else if( hh>=2 && hh<3 )
      {
        g = c;
        b = x;
      }
      else if( hh>=3 && hh<4 )
      {
        g = x;
        b = c;
      }
      else if( hh>=4 && hh<5 )
      {
        r = x;
        b = c;
      }
      else
      {
        r = c;
        b = x;
      }
      float m = l-c/2;
      r += m;
      g += m;
      b += m;
      r *= 255.0;
      g *= 255.0;
      b *= 255.0;
      int ri = (int)(r);
      int gi = (int)(g);
      int bi = (int)(b);
      
}
