#include <cstdio>
#include <cstdlib>
#include <png.h>

#define ERR(...) { fprintf(stderr, __VA_ARGS__); exit(1); }

static png_uint_32 user_chunk_data[4];
int unknown_chunk_read_cb(png_structp ptr, png_unknown_chunkp chunk) {
	//png_uint_32 *my_chunk_data = (png_uint_32*)png_get_user_chunk_ptr(ptr);
	printf("Unknown chunk?\n");
	return 1;
}

void swap_and_premultiply_alpha_transform(png_structp ptr, png_row_infop row_info, png_bytep data) {
	for(unsigned int x = 0; x < row_info->width * 4; x += 4) {
		png_byte r, g, b, a;
		r = data[x+0]; g = data[x+1]; b = data[x+2]; a = data[x+3];
		data[x+0] = (b*a) / 0xff;
		data[x+1] = (g*a) / 0xff;
		data[x+2] = (r*a) / 0xff;
	}
}

int main(int argc, char **argv, char **envp) {
	if(argc < 2) {
		ERR("Syntax: %s infile\n", argv[0]);
	}

	char *infilename = argv[1];
	FILE *fp_in = fopen(infilename, "rb");
	if(!fp_in) {
		ERR("Could not open %s.\n", infilename);
	}

	png_byte header[8];
	fread(header, 1, 8, fp_in);
	if(png_sig_cmp(header, 0, 8) != 0) {
		ERR("This is not a PNG file.\n");
	}

	png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!read_ptr) ERR("Something bad happened.\n");

	png_infop read_info = png_create_info_struct(read_ptr);
	if(!read_info) {
		png_destroy_read_struct(&read_ptr, NULL, NULL);
		ERR("Something bad happened.\n");
	}

	png_infop read_end = png_create_info_struct(read_ptr);
	if(!read_end) {
		png_destroy_read_struct(&read_ptr, &read_info, NULL);
		ERR("Something bad happened.\n");
	}

	// Maybe error stuff (setjmp?)

	if(setjmp(png_jmpbuf(read_ptr))) {
		ERR("What the hell R.\n");
		return 1;
	}

	png_init_io(read_ptr, fp_in);
	png_set_sig_bytes(read_ptr, 8);
	png_set_read_user_chunk_fn(read_ptr, user_chunk_data, unknown_chunk_read_cb);

	// Handle ALL unknown chunks.
	png_set_keep_unknown_chunks(read_ptr, PNG_HANDLE_CHUNK_ALWAYS, NULL, 0);

	// Take the easy way out (maybe)
	//png_read_png(read_ptr, read_info, PNG_TRANSFORM_EXPAND, NULL);

	//png_bytep *read_rows = png_get_rows(read_ptr, read_info);

	png_read_info(read_ptr, read_info);

	png_uint_32 width = 0;// = png_get_image_width(read_ptr, read_info);
	png_uint_32 height = 0;// = png_get_image_height(read_ptr, read_info);
	int color_type;
	int bitdepth;
	int filter_method, compression_type, interlace_type;
	double gamma = 0.45455;
	png_get_IHDR(read_ptr, read_info, &width, &height, &bitdepth, &color_type, &interlace_type, &compression_type, &filter_method);

	if(color_type == PNG_COLOR_TYPE_PALETTE) {
		png_set_palette_to_rgb(read_ptr);
	} else if(color_type == PNG_COLOR_TYPE_GRAY) {
		if(bitdepth < 8)
			png_set_expand_gray_1_2_4_to_8(read_ptr);
		png_set_gray_to_rgb(read_ptr);
	}
	if(png_get_valid(read_ptr, read_info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(read_ptr);

	png_set_add_alpha(read_ptr, 0xff, PNG_FILLER_AFTER);
	png_set_read_user_transform_fn(read_ptr, swap_and_premultiply_alpha_transform);
	png_read_update_info(read_ptr, read_info);
	png_set_user_transform_info(read_ptr, NULL, 8, 4);
	if(png_get_valid(read_ptr, read_info, PNG_INFO_gAMA)) png_get_gAMA(read_ptr, read_info, &gamma);
	if(png_get_valid(read_ptr, read_info, PNG_INFO_cHRM)) {
		double wx,wy,rx,ry,gx,gy,bx,by;
		png_get_cHRM(read_ptr, read_info, &wx, &wy, &rx, &ry, &gx, &gy, &bx, &by);
		printf("cHRM: %f, %f, %f, %f, %f, %f, %f, %f\n", wx,wy,rx,ry,gx,gy,bx,by);
	}
	if(png_get_valid(read_ptr, read_info, PNG_INFO_sRGB)) {
		int intent;
		png_get_sRGB(read_ptr, read_info, &intent);
		printf("sRGB intent %d\n", intent);
	}
	if(png_get_valid(read_ptr, read_info, PNG_INFO_pHYs)) {
		png_uint_32 res_x, res_y;
		int unit_type;
		png_get_pHYs(read_ptr, read_info, &res_x, &res_y, &unit_type);
		printf("Physical info: xres %u yres %u unit type %d\n", res_x, res_y, unit_type);
	}

	// re-read the info
	png_get_IHDR(read_ptr, read_info, &width, &height, &bitdepth, &color_type, &interlace_type, &compression_type, &filter_method);

	png_bytep read_data = (png_bytep)png_malloc(read_ptr, png_get_rowbytes(read_ptr, read_info) * height);
	png_bytep read_rows[height];
	int bpr = png_get_rowbytes(read_ptr, read_info);
	for(unsigned int i = 0; i < height; i++) {
		read_rows[i] = read_data + i*bpr;
	}
	png_read_image(read_ptr, read_rows);
	//png_free(read_ptr, read_rows);

	png_read_end(read_ptr, read_end);

	printf("Height: %u, Width: %u depth %d color %d\n", height, width, bitdepth, color_type);

	/**************WRITE**************/
	FILE *fp_out = fopen("test.png", "wb");
	png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!write_ptr) ERR("Something bad happened.\n");

	png_infop write_info = png_create_info_struct(write_ptr);
	if(!write_info) {
		png_destroy_write_struct(&write_ptr, NULL);
		ERR("Something bad happened.\n");
	}

	png_infop write_end = png_create_info_struct(write_ptr);
	if(!write_end) {
		png_destroy_write_struct(&write_ptr, &write_info);
		ERR("Something bad happened.\n");
	}

	if(setjmp(png_jmpbuf(write_ptr))) {
		ERR("What the hell W.\n");
		return 1;
	}

	// If we don't write the header first, png_write_chunk (for our custom CgBI chunk) doesn't add a header.
	// png_write_sig() supposedly exists, even in the header file, but I can't for the life of me use it?
	fwrite(header, 1, 8, fp_out);
	png_init_io(write_ptr, fp_out);
	png_set_sig_bytes(write_ptr, 8);

	png_set_filter(write_ptr, 0, PNG_FILTER_NONE);

	// The default window size is 15 bits. Setting it to -15 causes zlib to discard the header and crc information.
	// This is critical to making a proper CgBI PNG
	png_set_compression_window_bits(write_ptr, -15);

	png_set_IHDR(write_ptr, write_info, width, height, bitdepth, color_type, interlace_type, compression_type, filter_method);

	// Standard Gamma
	png_set_gAMA(write_ptr, write_info, 0.45455);

	// Primary Chromaticities white_xy, red_xy, blue_xy, green_xy, in that order.
	png_set_cHRM(write_ptr, write_info, 0.312700, 0.329000, 0.640000, 0.330000, 0.300000, 0.600000, 0.150000, 0.060000);

	// Apple's PNGs have an sRGB intent of 0.
	png_set_sRGB(write_ptr, write_info, 0);
	// CgBI = 0x50 00 20 06

	//png_unknown_chunkp cgbi = (png_unknown_chunkp)png_malloc(write_ptr, sizeof(png_unknown_chunk));
	png_byte cname[] = {'C', 'g', 'B', 'I', '\0'};
	png_byte cdata[] = {0x50, 0x00, 0x20, 0x06};
	png_write_chunk(write_ptr, cname, cdata, 4);
	png_write_info(write_ptr, write_info);
	png_write_image(write_ptr, read_rows);
	png_write_end(write_ptr, write_info);
	fclose(fp_out);
	fclose(fp_in);


	return 0;
}
