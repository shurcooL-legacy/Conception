uniform sampler2DRect SourceTex;
uniform int Samples;

void main()
{
	vec3 total = texture2DRect(SourceTex, gl_FragCoord.xy).rgb;;

	if (Samples > 0)
	{
		for (int i = 0; i < Samples; ++i)
			total += texture2DRect(SourceTex, gl_FragCoord.xy - vec2(0, i)).rgb;

		total /= float(Samples + 1);
	}
	else if (Samples < 0)
	{
		for (int i = 0; i > Samples; --i)
			total += texture2DRect(SourceTex, gl_FragCoord.xy - vec2(0, i)).rgb;
			
		total /= float(-Samples + 1);
	}

	gl_FragColor = vec4(total, 1);
}
