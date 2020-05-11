#version 150 


in  vec4 vPosition;
in  vec3 vNormal;
out vec4 color;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 LightPosition;
uniform float Shininess;
uniform mat4 Translation;
uniform mat4 Rotation;
uniform mat4 ScaleMatrix;
void main()
{
    // Transform vertex  position into eye coordinates
    vec3 pos = (ModelView * vPosition).xyz;
	
    vec3 L = normalize( (LightPosition).xyz - pos ); //assuming light position provided in camera coordinates
    vec3 E = normalize( -pos );
    vec3 H = normalize( L + E );

    vec4 rotatedNormal = Rotation * vec4(vNormal.x, vNormal.y, vNormal.z, 0.0);

    vec4 normalizedRotatedNormal = normalize(rotatedNormal);

    // Transform vertex normal into eye coordinates
    //vec3 N = normalize( ModelView*vec4(vNormal, 0.0) ).xyz;
    vec3 N = normalize( ModelView*vec4(normalizedRotatedNormal) ).xyz;

    // Compute terms in the illumination equation
    vec4 ambient = AmbientProduct;

    float Kd = max( dot(L, N), 0.0 );
    vec4  diffuse = Kd*DiffuseProduct;


    float Ks = pow( max(dot(N, H), 0.0), Shininess );
    vec4  specular = Ks * SpecularProduct;
    
    if( dot(L, N) < 0.0 ) {
	specular = vec4(0.0, 0.0, 0.0, 1.0);
    } 

    gl_Position = Projection * ModelView * Translation * Rotation * ScaleMatrix * vPosition;

    color = ambient + diffuse + specular;
    color.a = 1.0;
}
