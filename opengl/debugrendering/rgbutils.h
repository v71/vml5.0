#pragma once

namespace vml
{
	namespace colors
	{
		
		typedef glm::vec4 RGBA;
		typedef glm::vec3 RGB;
		typedef glm::vec3 HSV;
		typedef glm::vec3 HSL;

		const static RGBA AliceBlue				= glm::vec4( 0.941177f,0.972549f,1.000000f,1.0f );
		const static RGBA AntiqueWhite			= glm::vec4( 0.980392f,0.921569f,0.843137f,1.0f );
		const static RGBA Aquamarine			= glm::vec4( 0.400000f,0.803922f,0.666667f,1.0f );
		const static RGBA Azure					= glm::vec4( 0.941177f,1.000000f,1.000000f,1.0f );
		const static RGBA Beige					= glm::vec4( 0.960784f,0.960784f,0.862745f,1.0f );
		const static RGBA Bisque				= glm::vec4( 1.000000f,0.894118f,0.768627f,1.0f );
		const static RGBA Black					= glm::vec4( 0.000000f,0.000000f,0.000000f,1.0f );
		const static RGBA BlanchedAlmond		= glm::vec4( 1.000000f,0.921569f,0.803922f,1.0f );
		const static RGBA Blue					= glm::vec4( 0.000000f,0.000000f,1.000000f,1.0f );
		const static RGBA BlueViolet			= glm::vec4( 0.541177f,0.168627f,0.886275f,1.0f );
		const static RGBA Brown					= glm::vec4( 0.647059f,0.164706f,0.164706f,1.0f );
		const static RGBA Burlywood				= glm::vec4( 0.870588f,0.721569f,0.529412f,1.0f );
		const static RGBA CadetBlue				= glm::vec4( 0.372549f,0.619608f,0.627451f,1.0f );
		const static RGBA Chartreuse			= glm::vec4( 0.498039f,1.000000f,0.000000f,1.0f );
		const static RGBA Chocolate				= glm::vec4( 0.823529f,0.411765f,0.117647f,1.0f );
		const static RGBA Coral					= glm::vec4( 1.000000f,0.498039f,0.313726f,1.0f );
		const static RGBA CornFlowerBlue		= glm::vec4( 0.392157f,0.584314f,0.929412f,1.0f );
		const static RGBA CornSilk				= glm::vec4( 1.000000f,0.972549f,0.862745f,1.0f );
		const static RGBA Cyan					= glm::vec4( 0.000000f,1.000000f,1.000000f,1.0f );
		const static RGBA DarkGreen				= glm::vec4( 0.000000f,0.392157f,0.000000f,1.0f );
		const static RGBA DarkGoldenRod			= glm::vec4( 0.721569f,0.525490f,0.043137f,1.0f );
		const static RGBA DarkOliveGreen		= glm::vec4( 0.333333f,0.419608f,0.184314f,1.0f );
		const static RGBA DarkOrange			= glm::vec4( 1.000000f,0.549020f,0.000000f,1.0f );
		const static RGBA DarkSlateBlue			= glm::vec4( 0.282353f,0.239216f,0.545098f,1.0f );
		const static RGBA DarkIndianRed			= glm::vec4( 0.603922f,0.160784f,0.160784f,1.0f );
		const static RGBA DarkKhaki				= glm::vec4( 0.741176f,0.717647f,0.419608f,1.0f );
		const static RGBA DarkOrchid			= glm::vec4( 0.600000f,0.196078f,0.800000f,1.0f );
		const static RGBA DarkSeaGreen			= glm::vec4( 0.560784f,0.737255f,0.560784f,1.0f );
		const static RGBA DarkSlateGray			= glm::vec4( 0.192157f,0.309804f,0.309804f,1.0f );
		const static RGBA DarkTurquoise			= glm::vec4( 0.000000f,0.807843f,0.819608f,1.0f );
		const static RGBA DarkViolet			= glm::vec4( 0.580392f,0.000000f,0.827451f,1.0f );
		const static RGBA DarkSalmon			= glm::vec4( 0.913725f,0.588235f,0.478431f,1.0f );
		const static RGBA DeepPink				= glm::vec4( 1.000000f,0.078431f,0.576471f,1.0f );
		const static RGBA DeepSkyBlue			= glm::vec4( 0.000000f,0.749020f,1.000000f,1.0f );
		const static RGBA DimGray				= glm::vec4( 0.411765f,0.411765f,0.411765f,1.0f );
		const static RGBA DodgerBlue			= glm::vec4( 0.117647f,0.564706f,1.000000f,1.0f );
		const static RGBA FireBrick				= glm::vec4( 0.698039f,0.133333f,0.133333f,1.0f );
		const static RGBA FloralWhite			= glm::vec4( 1.000000f,0.980392f,0.941176f,1.0f );
		const static RGBA ForestGreen			= glm::vec4( 0.133333f,0.545098f,0.133333f,1.0f );
		const static RGBA Gainsboro				= glm::vec4( 0.862745f,0.862745f,0.862745f,1.0f );
		const static RGBA Gold					= glm::vec4( 1.000000f,0.843137f,0.000000f,1.0f );
		const static RGBA Goldenrod				= glm::vec4( 0.854902f,0.647059f,0.125490f,1.0f );
		const static RGBA Gray					= glm::vec4( 0.745098f,0.745098f,0.745098f,1.0f );
		const static RGBA Green					= glm::vec4( 0.000000f,1.000000f,0.000000f,1.0f );
		const static RGBA Grey					= glm::vec4( 0.500000f,0.500000f,0.500000f,1.0f );
		const static RGBA GreenYellow			= glm::vec4( 0.678431f,1.000000f,0.184314f,1.0f );
		const static RGBA Honeydew				= glm::vec4( 0.941176f,1.000000f,0.941176f,1.0f );
		const static RGBA HotPink				= glm::vec4( 1.000000f,0.411765f,0.705882f,1.0f );
		const static RGBA IndianRed				= glm::vec4( 0.803922f,0.360784f,0.360784f,1.0f );
		const static RGBA Indigo				= glm::vec4( 0.294118f,0.000000f,0.509804f,1.0f );
		const static RGBA Ivory					= glm::vec4( 1.000000f,1.000000f,0.941176f,1.0f );
		const static RGBA Khaki					= glm::vec4( 0.941177f,0.901961f,0.549020f,1.0f );
		const static RGBA Lavender				= glm::vec4( 0.901961f,0.901961f,0.980392f,1.0f );
		const static RGBA LavenderBlush			= glm::vec4( 1.000000f,0.941176f,0.960784f,1.0f );
		const static RGBA LawnGreen				= glm::vec4( 0.486275f,0.988235f,0.000000f,1.0f );
		const static RGBA LemonChiffon			= glm::vec4( 1.000000f,0.980392f,0.803922f,1.0f );
		const static RGBA LightBlue				= glm::vec4( 0.678431f,0.847059f,0.901961f,1.0f );
		const static RGBA LightCoral			= glm::vec4( 0.941176f,0.501961f,0.501961f,1.0f );
		const static RGBA LightCyan				= glm::vec4( 0.878431f,1.000000f,1.000000f,1.0f );
		const static RGBA LightGoldenrod		= glm::vec4( 0.933333f,0.866667f,0.509804f,1.0f );
		const static RGBA LightGoldenrodYellow  = glm::vec4( 0.980392f,0.980392f,0.823529f,1.0f );
		const static RGBA LightGray				= glm::vec4( 0.827451f,0.827451f,0.827451f,1.0f );
		const static RGBA LimeGreen				= glm::vec4( 0.196078f,0.803922f,0.196078f,1.0f );
		const static RGBA LightPink				= glm::vec4( 1.000000f,0.713726f,0.756863f,1.0f );
		const static RGBA LightSalmon			= glm::vec4( 1.000000f,0.627451f,0.478431f,1.0f );
		const static RGBA LightSeaGreen			= glm::vec4( 0.125490f,0.698039f,0.666667f,1.0f );
		const static RGBA LightSteelBlue	    = glm::vec4( 0.690196f,0.768627f,0.870588f,1.0f );
		const static RGBA LightSlateGray	    = glm::vec4( 0.466667f,0.533333f,0.600000f,1.0f );
		const static RGBA LightSkyBlue		    = glm::vec4( 0.529412f,0.807843f,0.980392f,1.0f );
		const static RGBA LightYellow		    = glm::vec4( 1.000000f,1.000000f,0.878431f,1.0f );
		const static RGBA Linen				    = glm::vec4( 0.941176f,0.941176f,0.901961f,1.0f );
		const static RGBA Magenta				= glm::vec4( 1.000000f,0.000000f,1.000000f,1.0f );
		const static RGBA MediumAquamarine		= glm::vec4( 0.400000f,0.803922f,0.666667f,1.0f );
		const static RGBA MediumBlue			= glm::vec4( 0.000000f,0.000000f,0.803922f,1.0f );
		const static RGBA MediumPurple			= glm::vec4( 0.576471f,0.439216f,0.858824f,1.0f );
		const static RGBA MediumSlateBlue		= glm::vec4( 0.482353f,0.407843f,0.933333f,1.0f );
		const static RGBA MidnightBlue			= glm::vec4( 0.098039f,0.098039f,0.439216f,1.0f );
		const static RGBA MintCream				= glm::vec4( 0.960784f,1.000000f,0.980392f,1.0f );
		const static RGBA MistyRose				= glm::vec4( 1.000000f,0.894118f,0.882353f,1.0f );
		const static RGBA Moccasin				= glm::vec4( 1.000000f,0.894118f,0.709804f,1.0f );
		const static RGBA Maroon				= glm::vec4( 0.690196f,0.188235f,0.376471f,1.0f );
		const static RGBA MediumVioletRed		= glm::vec4( 0.780392f,0.082352f,0.521569f,1.0f );
		const static RGBA MediumTurquoise		= glm::vec4( 0.282353f,0.819608f,0.800000f,1.0f );
		const static RGBA MediumSeaGreen		= glm::vec4( 0.235294f,0.701961f,0.443137f,1.0f );
		const static RGBA MediumSpringGreen		= glm::vec4( 0.000000f,0.980392f,0.603922f,1.0f );
		const static RGBA MediumOrchid			= glm::vec4( 0.729412f,0.333333f,0.827451f,1.0f );
		const static RGBA NavyBlue			    = glm::vec4( 0.000000f,0.000000f,0.501961f,1.0f );
		const static RGBA OldLace				= glm::vec4( 0.992157f,0.960784f,0.901961f,1.0f );
		const static RGBA Orchid			    = glm::vec4( 0.933333f,0.509804f,0.933333f,1.0f );
		const static RGBA Orange			    = glm::vec4( 1.000000f,0.647059f,0.000000f,1.0f );
		const static RGBA OliveDrab				= glm::vec4( 0.419608f,0.556863f,0.137255f,1.0f );
		const static RGBA OrangeRed			    = glm::vec4( 1.000000f,0.270588f,0.000000f,1.0f );
		const static RGBA PaleGreen				= glm::vec4( 0.596078f,0.984314f,0.596078f,1.0f );
		const static RGBA PaleTurquoise			= glm::vec4( 0.686275f,0.933333f,0.933333f,1.0f );
		const static RGBA PapayaWhip			= glm::vec4( 1.000000f,0.937255f,0.835294f,1.0f );
		const static RGBA PeachPuff				= glm::vec4( 1.000000f,0.854902f,0.725490f,1.0f );
		const static RGBA Pink					= glm::vec4( 1.000000f,0.752941f,0.796078f,1.0f );
		const static RGBA Plum					= glm::vec4( 0.866667f,0.627451f,0.866667f,1.0f );
		const static RGBA Purple				= glm::vec4( 0.627451f,0.125490f,0.941177f,1.0f );
		const static RGBA PaleVioletRed			= glm::vec4( 0.858824f,0.439216f,0.576471f,1.0f );
		const static RGBA PaleGoldenrod			= glm::vec4( 0.933333f,0.909804f,0.666667f,1.0f );
		const static RGBA PowderBlue			= glm::vec4( 0.690196f,0.878431f,0.901961f,1.0f );
		const static RGBA Peru					= glm::vec4( 0.803922f,0.521569f,0.247059f,1.0f );
		const static RGBA Red				    = glm::vec4( 1.000000f,0.000000f,0.000000f,1.0f );
		const static RGBA Rose				    = glm::vec4( 1.000000f,0.933333f,0.882353f,1.0f );
		const static RGBA RoyalBlue			    = glm::vec4( 0.254902f,0.411765f,0.882353f,1.0f );
		const static RGBA RosyBrown			    = glm::vec4( 0.737255f,0.560784f,0.560784f,1.0f );
		const static RGBA Tan				    = glm::vec4( 0.823529f,0.705882f,0.549020f,1.0f );
		const static RGBA Tomato		        = glm::vec4( 1.000000f,0.388235f,0.278431f,1.0f );
		const static RGBA Turquoise			    = glm::vec4( 0.250980f,0.878431f,0.815686f,1.0f );
		const static RGBA Violet			    = glm::vec4( 0.580392f,0.000000f,0.827451f,1.0f );
		const static RGBA VioletRed             = glm::vec4( 0.815686f,0.125490f,0.564706f,1.0f );
		const static RGBA SeaShell			    = glm::vec4( 1.000000f,0.960784f,0.933333f,1.0f );
		const static RGBA Salmon		        = glm::vec4( 0.980392f,0.501961f,0.447059f,1.0f );
		const static RGBA SandyBrown	        = glm::vec4( 0.956863f,0.643137f,0.376471f,1.0f );
		const static RGBA SeaGreen				= glm::vec4(0.180392f, 0.545098f,0.341176f,1.0f );
		const static RGBA Seashell			    = glm::vec4( 1.000000f,0.960784f,0.933333f,1.0f );
		const static RGBA Sienna		        = glm::vec4( 0.627451f,0.321569f,0.176471f,1.0f );
		const static RGBA SkyBlue			    = glm::vec4( 0.529412f,0.807843f,0.921569f,1.0f );
		const static RGBA SlateBlue				= glm::vec4( 0.415686f,0.352941f,0.803922f,1.0f );
		const static RGBA SlateGray				= glm::vec4( 0.439216f,0.541176f,0.564706f,1.0f );
		const static RGBA Snow					= glm::vec4( 1.000000f,0.980392f,0.980392f,1.0f );
		const static RGBA SpringGreen			= glm::vec4( 0.000000f,1.000000f,0.498039f,1.0f );
		const static RGBA SteelBlue				= glm::vec4( 0.274510f,0.509804f,0.705882f,1.0f );
		const static RGBA SaddleBrown			= glm::vec4( 0.545098f,0.270588f,0.074509f,1.0f );
		const static RGBA Thistle				= glm::vec4( 0.847059f,0.74902f, 0.847059f,1.0f );
		const static RGBA Wheat				    = glm::vec4( 0.960784f,0.870588f,0.701961f,1.0f );
		const static RGBA White				    = glm::vec4( 1.000000f,1.000000f,1.000000f,1.0f );
		const static RGBA WhiteSmoke		    = glm::vec4( 0.960784f,0.960784f,0.960784f,1.0f );
		const static RGBA Yellow				= glm::vec4( 1.000000f,1.000000f,0.000000f,1.0f );
		const static RGBA YellowGreen			= glm::vec4( 0.603922f,0.803922f,0.196078f,1.0f );

		/////////////////////////////////////////////////////////////////////////////////////////
		// colors conversion functions
		
		static vml::colors::RGB GetLuminance(const vml::colors::RGB& rgb)
		{
			return glm::vec3(0.2126f * rgb.r + 0.7152f * rgb.g + 0.0722f * rgb.b);
		}
		
		/////////////////////////////////////////////////////////////////////////////////////////
		// convert from hsv to rgb

		static vml::colors::RGB HsvToRgb(const vml::colors::HSV &hsv)
		{
			vml::colors::RGB rgb(0,0,0);
			unsigned char region, p, q, t;
			unsigned int h, s, v, remainder;

			if (hsv.s == 0)
			{
				rgb.r = hsv.z;
				rgb.g = hsv.z;
				rgb.b = hsv.z;
				return rgb;
			}

			// converting to 16 bit to prevent overflow
			// h s v = x y z

			h = hsv.x;
			s = hsv.y;
			v = hsv.z;

			region = h / 43;
			remainder = (h - (region * 43)) * 6;

			p = (v * (255 - s)) >> 8;
			q = (v * (255 - ((s * remainder) >> 8))) >> 8;
			t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

			switch (region)
			{
				case 0:	 rgb.r = v;	rgb.g = t; rgb.b = p; break;
				case 1:	 rgb.r = q;	rgb.g = v; rgb.b = p; break;
				case 2:	 rgb.r = p;	rgb.g = v; rgb.b = t; break;
				case 3:	 rgb.r = p;	rgb.g = q; rgb.b = v; break;
				case 4:	 rgb.r = t;	rgb.g = p; rgb.b = v; break;
				default: rgb.r = v;	rgb.g = p; rgb.b = q; break;
			}
			return rgb;
		}

		////////////////////////////////////////////////////////////////////////
		// converts from rgb to hsv

		static vml::colors::HSV RgbToHsv(const vml::colors::RGB &rgb)
		{
			vml::colors::HSV hsv(0,0,0);
			unsigned char rgbMin, rgbMax;

			rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
			rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

			// h s v = x y z

			hsv.z = rgbMax;

			if (hsv.z == 0)
			{
				hsv.x = 0;
				hsv.y = 0;
				return hsv;
			}

			hsv.s = 255 * ((long)(rgbMax - rgbMin)) / hsv.z;
			
			if (hsv.s == 0)
			{
				hsv.x = 0;
				return hsv;
			}

			if (rgbMax == rgb.r)
				hsv.x = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
			else if (rgbMax == rgb.g)
				hsv.x = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
			else
				hsv.x = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

			return hsv;
		}
		
		////////////////////////////////////////////////////////////////////////////////////////
		/// Conversion functions between RGB and other color systems.
		/// These functions are duplicated from the fantastic python module colorsys.
		/// http://en.wikipedia.org/wiki/YIQ
		/// http://en.wikipedia.org/wiki/HLS_color_space
		/// http://en.wikipedia.org/wiki/HSV_color_space

		/// YIQ: used by composite video signals (linear combinations of RGB)
		/// Y: perceived grey level (0.0 == black, 1.0 == white)
		/// I, Q: color components
		
		static glm::vec3 RgbToYiq(const vml::colors::RGB& color)
		{
			return glm::vec3(0.30f*color[0] + 0.59f*color[1] + 0.11f*color[2],
							 0.60f*color[0] - 0.28f*color[1] - 0.32f*color[2],
							 0.21f*color[0] - 0.52f*color[1] + 0.31f*color[2]);
		}

		static glm::vec3 YiqToRgb(const vml::colors::RGB& color)
		{
			glm::vec3 c(color[0] + 0.948262f*color[1] + 0.624013f*color[2],
					    color[0] - 0.276066f*color[1] - 0.639810f*color[2],
					    color[0] - 1.105450f*color[1] + 1.729860f*color[2]);
			c[0] < 0.0f ? 0.0f : c[0] < 1.0f ? c[0] : 1.0f;
			c[1] < 0.0f ? 0.0f : c[1] < 1.0f ? c[1] : 1.0f;
			c[2] < 0.0f ? 0.0f : c[2] < 1.0f ? c[2] : 1.0f;
			return c;
		}

		static float HSL2RGBMix( float m1, float m2, float hue)
		{
			const float ONE_SIXTH = 1.0f / 6.0f;
			const float TWO_THIRD = 2.0f / 3.0f;

			assert(hue >= -1.0 && hue <= 1.0);
			if (hue < 0.0) hue += 1.0f;

			if (hue < ONE_SIXTH)
				return m1 + (m2 - m1) * hue * 6.0f;

			if (hue < 0.5f)
				return m2;

			if (hue < TWO_THIRD)
				return m1 + (m2 - m1) * (TWO_THIRD - hue) * 6.0f;

			return m1;
		}
		
		////////////////////////////////////////////////////////////////////
		// Helper for HslToRgba
		
		static float HueToRgb(const float p, const float q, float t)
		{
			if (t < 0.0f) t += 1.0f;
			if (t > 1.0f) t -= 1.0f;
			if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
			if (t < 1.0f / 2.0f) return q;
			if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
			return p;
		}

		////////////////////////////////////////////////////////////////////
		/// Converts an HSL color value to RGB.
		/// Input: Vector4 ( X: [0.0, 1.0], Y: [0.0, 1.0], Z: [0.0, 1.0], W: [0.0, 1.0] )**strong text**
		/// Output: Color ( R: [0.0, 1.0], G: [0.0, 1.0], B: [0.0, 1.0], A: [0.0, 1.0] )
		
		static glm::vec4 HslToRgba(const glm::vec4 &hsl)
		{
			float r, g, b;

			if (hsl.y == 0.0f)
				r = g = b = hsl.z;
			else
			{
				float q = hsl.z < 0.5f ? hsl.z * (1.0f + hsl.y) : hsl.z + hsl.y - hsl.z * hsl.y;
				float p = 2.0f * hsl.z - q;
				r = HueToRgb(p, q, hsl.x + 1.0f / 3.0f);
				g = HueToRgb(p, q, hsl.x);
				b = HueToRgb(p, q, hsl.x - 1.0f / 3.0f);
			}

			return glm::vec4(r, g, b, hsl.w);
		}

		////////////////////////////////////////////////////////////////////
		/// Converts an RGB color value to HSL.
		/// Input: Color ( R: [0.0, 1.0], G: [0.0, 1.0], B: [0.0, 1.0], A: [0.0, 1.0] )
		/// Output: Vector4 ( X: [0.0, 1.0], Y: [0.0, 1.0], Z: [0.0, 1.0], W: [0.0, 1.0] )
	
		static glm::vec4 RgbaToHsl(const glm::vec4 &rgba)
		{

			float max = (rgba.r > rgba.g && rgba.r > rgba.b) ? rgba.r :
				(rgba.g > rgba.b) ? rgba.g : rgba.b;
			float min = (rgba.r < rgba.g && rgba.r < rgba.b) ? rgba.r :
				(rgba.g < rgba.b) ? rgba.g : rgba.b;

			float h, s, l;
			h = s = l = (max + min) / 2.0f;

			if (max == min)
				h = s = 0.0f;
			else
			{
				float d = max - min;
				s = (l > 0.5f) ? d / (2.0f - max - min) : d / (max + min);

				if (rgba.r > rgba.g && rgba.r > rgba.b)
					h = (rgba.g - rgba.b) / d + (rgba.g < rgba.b ? 6.0f : 0.0f);

				else if (rgba.g > rgba.b)
					h = (rgba.b - rgba.r) / d + 2.0f;

				else
					h = (rgba.r - rgba.g) / d + 4.0f;

				h /= 6.0f;
			}

			return glm::vec4(h, s, l, rgba.a);
		}
	

	} // end of colors namespace

} // end of vml namespace
