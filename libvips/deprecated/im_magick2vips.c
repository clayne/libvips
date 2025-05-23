/* Read a file using libMagick
 *
 * 17/12/11
 * 	- just a stub
 */

/*

	This file is part of VIPS.

	VIPS is free software; you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
	02110-1301  USA

 */

/*

	These files are distributed with VIPS - http://www.vips.ecs.soton.ac.uk

 */

/* Turn on debugging output.
#define DEBUG
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /*HAVE_CONFIG_H*/
#include <glib/gi18n-lib.h>

#include <vips/vips.h>

int
im_magick2vips(const char *filename, IMAGE *out)
{
	VipsImage *t;

	/* Old behaviour was always to read all frames.
	 */
	if (vips_magickload(filename, &t, "n", -1, NULL))
		return -1;
	if (vips_image_write(t, out)) {
		g_object_unref(t);
		return -1;
	}
	g_object_unref(t);

	return 0;
}

static int
ismagick(const char *filename)
{
	return vips_foreign_is_a("magickload", filename);
}

static const char *magick_suffs[] = { NULL };

/* magick format adds no new members.
 */
typedef VipsFormat VipsFormatMagick;
typedef VipsFormatClass VipsFormatMagickClass;

static void
vips_format_magick_class_init(VipsFormatMagickClass *class)
{
	VipsObjectClass *object_class = (VipsObjectClass *) class;
	VipsFormatClass *format_class = (VipsFormatClass *) class;

	object_class->nickname = "magick";
	object_class->description = _("libMagick-supported");

	format_class->is_a = ismagick;
	format_class->load = im_magick2vips;
	format_class->suffs = magick_suffs;

	/* This can be very slow :-( Use our own jpeg/tiff/png etc. loaders in
	 * preference if we can.
	 */
	format_class->priority = -1000;
}

static void
vips_format_magick_init(VipsFormatMagick *object)
{
}

G_DEFINE_TYPE(VipsFormatMagick, vips_format_magick, VIPS_TYPE_FORMAT);

int
im_bufmagick2vips(void *buf, size_t len, IMAGE *out, gboolean header_only)
{
	VipsImage *t;

	/* header_only is automatic ... this call will only decompress on
	 * pixel access.
	 */

	if (vips_magickload_buffer(buf, len, &t, NULL))
		return -1;
	if (vips_image_write(t, out)) {
		g_object_unref(t);
		return -1;
	}
	g_object_unref(t);

	return 0;
}
