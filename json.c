#include <yajl/yajl_parse.h>
#include <yajl/yajl_gen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int reformat_null(void * ctx)
{
	yajl_gen g = (yajl_gen) ctx;
	return yajl_gen_status_ok == yajl_gen_null(g);
}

static int reformat_boolean(void * ctx, int boolean)
{
	yajl_gen g = (yajl_gen) ctx;
	return yajl_gen_status_ok == yajl_gen_bool(g, boolean);
}

static int reformat_number(void * ctx, const char * s, size_t l)
{
	yajl_gen g = (yajl_gen) ctx;
	return yajl_gen_status_ok == yajl_gen_number(g, s, l);
}

static int reformat_string(void * ctx, const unsigned char * stringVal,
		size_t stringLen)
{
	yajl_gen g = (yajl_gen) ctx;
	return yajl_gen_status_ok == yajl_gen_string(g, stringVal, stringLen);
}

static int reformat_map_key(void * ctx, const unsigned char * stringVal,
		size_t stringLen)
{
	yajl_gen g = (yajl_gen) ctx;
	return yajl_gen_status_ok == yajl_gen_string(g, stringVal, stringLen);
}

static int reformat_start_map(void * ctx)
{
	yajl_gen g = (yajl_gen) ctx;
	return yajl_gen_status_ok == yajl_gen_map_open(g);
}


static int reformat_end_map(void * ctx)
{
	yajl_gen g = (yajl_gen) ctx;
	return yajl_gen_status_ok == yajl_gen_map_close(g);
}

static int reformat_start_array(void * ctx)
{
	yajl_gen g = (yajl_gen) ctx;
	return yajl_gen_status_ok == yajl_gen_array_open(g);
}

static int reformat_end_array(void * ctx)
{
	yajl_gen g = (yajl_gen) ctx;
	return yajl_gen_status_ok == yajl_gen_array_close(g);
}

static yajl_callbacks callbacks = {
	reformat_null,
	reformat_boolean,
	NULL,
	NULL,
	reformat_number,
	reformat_string,
	reformat_start_map,
	reformat_map_key,
	reformat_end_map,
	reformat_start_array,
	reformat_end_array
};
static void usage(const char * progname)
{
	fprintf(stderr, "%s: reformat json from stdin\n"
			"usage:  json_reformat [options]\n"
			"    -m minimize json rather than beautify (default)\n"
			"    -u allow invalid UTF8 inside strings during parsing\n",
			progname);
	exit(1);

}


int main(int argc, char ** argv) {


	char sData[]="{\"Message\":\"Hello\",\"StatusCode\":400,\"bool\":true,\"int\":43,\"double\":3.21, \"null\":null}";
	size_t rd = strlen(sData);
	const unsigned char *c_data = (const unsigned char *)sData;
	yajl_handle hand;
	yajl_gen g;
	yajl_status stat;	

        g = yajl_gen_alloc(NULL);

	hand = yajl_alloc(&callbacks, NULL, (void *)g);

        stat = yajl_parse(hand, c_data, rd);

	stat = yajl_complete_parse(hand);

	if (stat != yajl_status_ok){ 
		unsigned char * str = yajl_get_error(hand, 1, c_data, rd);
		printf("Yajl error Invalid Json \n");
		yajl_free_error(hand, str);
		
	}
	else
	{
		const unsigned char * buf;
		size_t len;
		yajl_gen_get_buf(g, &buf, &len);
		printf("%s\n",buf);  
		yajl_gen_clear(g);
	}
	yajl_gen_free(g);
	yajl_free(hand);
         
	return 0;
}
