#version 330

uniform sampler2D textureMap0;
// Drugi plik do ustawiania cieniowania
out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela (Phonga)

// Deklaracja dw?ch ?r?del ?wiat?a
in vec4 l;
in vec4 l2;

in vec4 n;
in vec4 v;
in vec2 iTexCoord0;


void main(void) {

//pobieramy l n i v do lokalnych zmiennych i normalizujemy
vec4 ml = normalize(l);
vec4 ml2 = normalize(l2);

//Bez zmian
vec4 mn = normalize(n);
vec4 mv = normalize(v);
vec4 kd=texture(textureMap0,iTexCoord0);
vec4 ks= vec4(1,1,1,1);



vec4 mr = reflect(-ml,mn);
vec4 mr2 = reflect(-ml2,mn);

 float nl = clamp( dot(mn,ml),0,1); 
 float n2 = clamp( dot(mn,ml2),0,1); 

  //zakladamy ze kolory ls i ks to kolor bialy wiec brak we wzorze
  float rv = pow(clamp(dot(mr,mv),0,1), 25); 
  float rv2 = pow(clamp(dot(mr2,mv),0,1), 25); 

 
    pixelColor= vec4(kd.rgb * nl,kd.a)+vec4(ks.rgb*rv,0) + vec4(kd.rgb * n2,kd.a)+vec4(ks.rgb*rv2,0);
}