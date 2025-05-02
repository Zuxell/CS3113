/*
uniform sampler2D diffuse;
varying vec2 texCoordVar;

void main() {
    gl_FragColor = texture2D(diffuse, texCoordVar);
}
*/

 // fragment_lit.glsl
 uniform sampler2D diffuse;
 uniform vec2 lightPosition;

 varying vec2 texCoordVar;
 varying vec2 varPosition;

uniform int is_dark;


 float attenuate(float dist, float a, float b)
 {
      return 1.0 / (1.0 + (a * dist) + (b * dist  * dist));
 }

 void main()
 {
     // The brightness is directly based on the distance between the light source's
     // location and the pixel's location
     
     if (is_dark == 1){
         //float brightness = attenuate(distance(lightPosition, varPosition), 1.0, 0.0);
         float brightness = 0.5;
         vec4 color = texture2D(diffuse, texCoordVar);
         gl_FragColor = vec4(color.rgb * brightness, color.a);
     }
     else{
         gl_FragColor = texture2D(diffuse, texCoordVar);
     }

 }

