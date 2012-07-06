#include "Main.h"

GLuint	oFontBase[1];

const double fontWidth[1] = { charWidth };

// print text on screen
void OglUtilsPrint(double X, double Y, int iFont, PrintAlignment nAlignment, const char *chText)
{
	assert(0 == iFont);

	if (chText == NULL)
		return;

	//char *chAsciiCodes = (char *)malloc(sizeof(char) * (strlen(chText) + 1));

	//glLoadIdentity();
	glEnable(GL_BLEND);
	//glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 1);

	//vsprintf(chAsciiCodes, chText, NULL);

	glPushMatrix();
	if (LEFT == nAlignment)
		glTranslated(X, Y, 0);
	else if (CENTER == nAlignment)
		glTranslated(X - (strlen(chText)+0) * 0.5 * fontWidth[iFont], Y, 0);
	else if (RIGHT == nAlignment)
		glTranslated(X - (strlen(chText)+0) * fontWidth[iFont], Y, 0);

	//const double Scale = 1.05; glScaled(Scale, Scale, Scale);
	glTranslated(-4, -1, 0);

	glListBase(oFontBase[iFont]);
	//glCallLists(strlen(chAsciiCodes), GL_UNSIGNED_BYTE, chAsciiCodes);
	glCallLists((GLsizei)strlen(chText), GL_UNSIGNED_BYTE, chText);
	glPopMatrix();

	//glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	//free(chAsciiCodes);
}

// init the font
bool OglUtilsInitFont()
{
	float fCharX;
	float fCharY;

	// Load texture
	glBindTexture(GL_TEXTURE_2D, 1);
	if (GL_TRUE == glfwLoadTexture2D("data/Font2048.tga", GLFW_BUILD_MIPMAPS_BIT))
	{
		printf("Loaded texture successfully.\n");
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		printf("Error: Failed to load texture.\n");
		return false;
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Font 1
	oFontBase[0] = glGenLists(256);

	for (int iLoop1 = 0; iLoop1 < 256; iLoop1++)
	{
		fCharX = float(iLoop1 % 16) / 16.0f;
		fCharY = float(iLoop1 / 16) / 16.0f;

		glNewList(oFontBase[0] + iLoop1, GL_COMPILE);
			//const float offset = 0.002f;
			const float offset = 0.004f;
			glBegin(GL_QUADS);
				glTexCoord2f(fCharX+offset, 1 - fCharY - 0.0625f+offset);
				glVertex2i(0, 16);
				glTexCoord2f(fCharX + 0.0625f-offset, 1 - fCharY - 0.0625f+offset);
				glVertex2i(16, 16);
				glTexCoord2f(fCharX + 0.0625f-offset, 1 - fCharY-offset);
				glVertex2i(16, 0);
				glTexCoord2f(fCharX+offset, 1 - fCharY-offset);
				glVertex2i(0, 0);
			glEnd();
			glTranslated(fontWidth[0], 0.0, 0.0);
		glEndList();
	}

	return true;
}

// kill the font
void OglUtilsKillFont()
{
	glDeleteLists(oFontBase[0], 256);
}
