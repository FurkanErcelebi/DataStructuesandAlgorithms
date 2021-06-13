#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX 256

int width = 0;
int height = 0;
int max_value,max_repeate;
unsigned char **pgm_image_matrix;


int getValue(char *str){
	int val = -1;
	int size = strlen(str),i;
	
	for(i = 0;i < size;i++){
		if( 58 > str[i] && 47 < str[i] ){
			if(i == 0){
				val = str[i] - 48;
			}
			else {
				val = (10 * val) + str[i] - 48;
			}
		}
	}
	
	return val;
}

int getMaxValue(){
	
	int i,j;
	
	unsigned char max = 0;
	for(i = 0;i < height;i++){
		for(j = 0;j < width;j++){
			if(max < pgm_image_matrix[i][j]){
				max = pgm_image_matrix[i][j];
			}
		}
	}
	
	return (int)max;
}

int getMaxSequanceRepeate(){
	
	int values[max_value + 1];
	int i,j;
	unsigned char val;
	
	for(i = 0;i < max_value + 1;i++){
		values[i] = 0;
	}
	
	val = pgm_image_matrix[0][0];
	for(i = 0;i < height;i++){
		for(j = 0;j < width;j++){
			if(val == pgm_image_matrix[i][j]){
				values[(int)val]++;
			}
			else{
				val = pgm_image_matrix[i][j];
				values[(int)val] = 1;
			}	
		}
	}
	
	int max = 0;
	for(i = 0;i < max_value;i++){
		if(max < values[i]){
			max = values[i];
		}
	}
	
	return max;
}

void eraseString(char *str,int size){
	
	int i;
	for(i = 0;i < size;i++){
		str[i] = 0;
	}
	
}

void eraseMatrix(){
	
	int i;
	for (i = 0; i < height; i++)
	{
	    free(pgm_image_matrix[i]);
	}
	free(pgm_image_matrix);
	
}

void getPGMMatrix(char *file_path){
	
	char line[MAX];
	char filter[2] = " ";
	char *str;
	FILE *pgm_file = fopen(file_path,"r");
	if(pgm_file == NULL){
		printf("File cannot opened");
		return;
	}
	
	int num_line = 3;
	int size,i,j,k,val;
	unsigned char cval;
	
	eraseMatrix();
	fgets(line,MAX,pgm_file);// leap magic number
	
	fgets(line,MAX,pgm_file);
	while(line[0] == '#'){
		fgets(line,MAX,pgm_file);
	}
	
	str = strtok(line,filter);
	width = atoi(str);
	str = strtok(NULL,filter);
	height = atoi(str);
	
	fgets(line,MAX,pgm_file);
	while(line[0] == '#'){
		fgets(line,MAX,pgm_file);
	}
			
	size = strlen(line);
        str = (char*)malloc(size * sizeof(char));
	strncpy(str,line,size);
	max_value = atoi(str);
	free(str);
	
	pgm_image_matrix = (unsigned char **)malloc(height * sizeof(unsigned char*));
	
	for(i = 0;i < height;i++){
		pgm_image_matrix[i] = (unsigned char*)malloc(width * sizeof(unsigned char));
	}
	
	i = 0;
	j = 0;
	do{
		
			fgets(line,MAX,pgm_file);
			if(line != NULL){
				
				num_line++;
				str =strtok(line,filter);
				while(str != NULL){
					val = getValue(str);
					if(0 <= val && 255 >= val){
						if(i < height){
						    pgm_image_matrix[i][j] = (unsigned char)val;
							if(j < width - 1){
								j++;
							}
							else{
								if(i < height){
									i++;	
								}
								j = 0;
							}	
						}
						else{
							printf("Pixel count exceeded\n\n");
						}
					}
					else if(val > 255){
						printf("Gecersiz deger bulunmaktadır\n");
						return ;
					}
					str = strtok(NULL,filter);
				}
				
			}
		
	}while(!feof(pgm_file));
	
	
	fclose(pgm_file);
	
}

void getPGMMatrixFromZip(char *file_path){
	
	unsigned char string[4],num;
	int k,i,j,count,max,format;
	
	FILE *zip_pgm_file = fopen(file_path,"rb");
	if(zip_pgm_file == NULL){
		printf("Dosya acilamadi\n");
		return;
	}
	
	eraseMatrix();
	
	format = (int)fgetc(zip_pgm_file);
	fread(string,sizeof(unsigned char),format * 2,zip_pgm_file);
	
	if(format == 2){
		height = ((int)string[1] * 256) + (int)string[0];
		width = ((int)string[3] * 256) + (int)string[2];
		
	}
	else{
		height = (int)string[0];
		width = (int)string[1];
	}
	
	
	
	pgm_image_matrix = (unsigned char **)malloc(height * sizeof(unsigned char *));
	
	for(k = 0;k < height;k++){
		pgm_image_matrix[k] = (unsigned char*)malloc(width * sizeof(unsigned char));
	}
	
			
	fread(string,sizeof(unsigned char),format * 2,zip_pgm_file);
	
	i = 0;
	j = 0;
	do{		
		
		count = 0;			
		num = string[0];
		
		if(format == 2){
			num = (string[1] * 256) + num;
		}

		count = (int)num;
		
		if(format == 2){
			num = string[2];
		}
		else{
			num = string[1];
		}
		
		for(k = 0;k < count;k++){
		
			pgm_image_matrix[i][j] = num;
			
			if(j < width - 1){
				j++;
			}
			else{
				i++;	
				j = 0;
			}
			
		}
						
		fread(string,sizeof(unsigned char),format * 2,zip_pgm_file);	
			
	}while(!feof(zip_pgm_file));
	
	fclose(zip_pgm_file);
	
}

void zipPgmWithRLE(char *path,char *file){
	
	int i,j,k,data,count,max,format,size;
	unsigned char value;
	char *zip_file;
	
	size = strlen(path) + 17;
	zip_file = (char *)malloc(size * sizeof(char));
	strcpy(zip_file,path);
	strcat(zip_file,"test_encoded.txt");
	
	FILE *pgm_zip_file = fopen(zip_file,"w");
	if(pgm_zip_file == NULL){
		printf("Dosya olusrutulamadi");
		return;
	}
	
	max = getMaxSequanceRepeate();
	
	if(max > 255 || height > 255 || width > 255){
		format = 2;
	}
	else{
		format = 1;
	}
	value = (int)format;
	fputc(value,pgm_zip_file);
	
	if(format == 2){
		fputc((unsigned char)height % 256,pgm_zip_file);
		fputc((unsigned char)(height >> 8),pgm_zip_file);
		fputc((unsigned char)width % 256,pgm_zip_file);
		fputc((unsigned char)(width >> 8),pgm_zip_file);
	}
	else{
		fputc((unsigned char)height,pgm_zip_file);
		fputc((unsigned char)width,pgm_zip_file);
	}
	
			
	value = pgm_image_matrix[0][0];
	count = 0; 
	for(i = 0;i < height;i++){
		for(j = 0;j < width;j++){
			if(value == pgm_image_matrix[i][j]){
				count++;
			}
			else{
			
				if(format == 2){
					fputc((unsigned char)count % 256,pgm_zip_file);
					fputc((unsigned char)(count >> 8),pgm_zip_file);
					fputc((unsigned char)value,pgm_zip_file);
					fputc((unsigned char)'\0',pgm_zip_file);
				}
				else{
					fputc((unsigned char)count,pgm_zip_file);
					fputc((unsigned char)value,pgm_zip_file);
				}
				
				value = pgm_image_matrix[i][j];
				count = 1; 
			}		
		}
	}
				
	if(format == 2){
		fputc((unsigned char)count % 256,pgm_zip_file);
		fputc((unsigned char)(count >> 8),pgm_zip_file);
		fputc((unsigned char)value,pgm_zip_file);
		fputc((unsigned char)'\0',pgm_zip_file);
	}
	else{
		fputc((unsigned char)count,pgm_zip_file);
		fputc((unsigned char)value,pgm_zip_file);
	}
	
	printf("\n%s dosyasi RLE metoduyla ziplendi\n",file);
	
	fclose(pgm_zip_file);

}

int controlZipFile(char *path){
	
	
	unsigned char string[4];
	int wid,heg,size,count,pre_value,num,num_exceed,sequencial,format;
	char *zip_file;
	
	size = strlen(path);
	zip_file = (char*)malloc((size + 17) * sizeof(char));
	strcpy(zip_file,path);
	strcat(zip_file,"test_encoded.txt");
	
	FILE *zip_pgm_file = fopen(zip_file,"rb");
	if(zip_pgm_file == NULL){
		printf("Dosya acilamadi\n");
		return 0;
	}
	
	format = (int)fgetc(zip_pgm_file);
	fread(string,sizeof(unsigned char),format * 2,zip_pgm_file);
	
	if(format == 2){
		heg = ((int)string[1] * 256) + (int)string[0];
		wid = ((int)string[3] * 256) + (int)string[2];
		
	}
	else{
		heg = (int)string[0];
		wid = (int)string[1];
	}
	
	num_exceed = 0;
	sequencial = 0;
	count = 0;
	pre_value = -1;
			
	fread(string,sizeof(unsigned char),format * 2,zip_pgm_file);

	do{	
			
		num = (int)string[0];	
		
		if(format == 2){
			num = ((int)string[1] * 256) + num;
		}

		count += num;
		
		if(format == 2){
			num = (int)string[3];
			if(0 < num && !num_exceed){
				num_exceed = 1;
			}
			num = (int)string[2];
			
		}
		else{
			num = (int)string[1];
		}
		
		if(pre_value != -1){
			if(pre_value == num && !sequencial){
				sequencial = 1;
			}
		}
		pre_value = num;
						
		fread(string,sizeof(unsigned char),format * 2,zip_pgm_file);	
			
	}while(!feof(zip_pgm_file));

	
	if(sequencial){
		printf("Run Lengthte %d arka rakya ayni deger bulunmakta!\n",num);
	}
	
	if(num_exceed){
		printf("Renk degeri 0 ile 255 arasinda degildir!\n");
	}
	
	if(count != (wid * heg)){
		printf("Matrix buyuklugu ve piksel sayisi uyusmuyor\n");
	}
	
	fclose(zip_pgm_file);
	
	if(count == (wid * heg) && !num_exceed && !sequencial){
		printf("ZIP dosyasi acilabilir\n");
		return 1;
	}

	return 0;
}

void extractZipPGM(char *path,char *file){
	
	unsigned char string[4],num;
	char *str,*line,*tmp,*pgm;
	int k,i,j,count,max,format,size;
	
	size =  strlen(path);
	tmp = (char*)malloc((size + 17) * sizeof(char));
	eraseString(tmp,(size + 17));
	strcpy(tmp,path);
	if(!controlZipFile(path)){
		return;
	}
	
	pgm = (char*)malloc((size + 17) * sizeof(char));
	eraseString(tmp,(size + 17));
	strcpy(pgm,tmp);
	strcat(tmp,file);
	strcat(pgm,"test_decoded.pgm");
	
	getPGMMatrixFromZip(tmp);
	
	FILE *one_file = fopen(pgm,"rb");
	if(one_file != NULL){
		remove(pgm);
	}
	
	FILE *pgm_file = fopen(pgm,"w");
	if(pgm_file == NULL){
		printf("Dosya acilamadi\n");
		return;
	}
	
	
	line = (char*)malloc(MAX * sizeof(char));
	str = (char*)malloc(MAX * sizeof(char));
	
	eraseString(str,MAX);
	strcpy(str,"P2\n");
	fputs(str,pgm_file);
	
	eraseString(line,MAX);
	eraseString(str,strlen(str));
	sprintf(str,"%d",width);
	strcpy(line,str);
	strcat(line," ");
	eraseString(str,strlen(str));
	sprintf(str,"%d",height);
	strcat(line,str);
	strcat(line,"\n");
	fputs(line,pgm_file);
	
	max_value = getMaxValue();
	eraseString(line,strlen(line));
	eraseString(str,strlen(str));
	sprintf(str,"%d",max_value);
	strcpy(line,str);
	strcat(line,"\n");
	fputs(line,pgm_file);
	
	for(i = 0;i < height;i++){
		for(j = 0;j < width;j++){
			sprintf(str,"%d",pgm_image_matrix[i][j]);
			strcpy(line,str);
			if(j < width - 1){
				strcat(line," ");
			}
			fputs(line,pgm_file);			
		}
		strcpy(str,"\n");
		fputs(str,pgm_file);
	}
	fclose(pgm_file);
	
	printf("\nZip dosyasi disa aktarildi\n");
	
}

void editPixels(char *path,char *file,unsigned char pixel1,unsigned char pixel2){
	
	unsigned char string[4];
	int format,num,count,size,pre_val,pre_index,index,i,is_changed;
	int extra_rle[MAX];
	char *zip_file,*tmp_file;
	
	size = strlen(path) + strlen(file);
	zip_file = (char *)malloc(size * sizeof(char));
	strcpy(zip_file,path);
	strcat(zip_file,file);
	
	for(i = 0;i < MAX;i++){
		extra_rle[i] = 0;
	}
	
	FILE *read_zip_file = fopen(zip_file,"rb");
	if(read_zip_file == NULL){
		printf("Dosya acilamadi\n");
		return;
	}
	
	size = strlen(path) + 9;
	tmp_file = (char *)malloc(size * sizeof(char));
	strcpy(tmp_file,path);
	strcat(tmp_file,"file.tmp");
	
	FILE *write_zip_file = fopen(tmp_file,"wb");
	if(write_zip_file == NULL){
		printf("Dosya olusturulamadi\n");
		return;
	}
	
	format = (int)fgetc(read_zip_file);
	fputc((unsigned char)format,write_zip_file);
	
	fread(string,sizeof(unsigned char),format * 2,read_zip_file);
	
	fputc(string[0],write_zip_file);
	fputc(string[1],write_zip_file);
	
	if(format == 2){
		fputc(string[2],write_zip_file);
		fputc(string[3],write_zip_file);
	}
	
	fread(string,sizeof(unsigned char),format * 2,read_zip_file);
	index = 0;
	pre_index = -1;
	is_changed = 0;
	do{		
		count = 0;
		num = (int)string[0];	
		
		if(format == 2){
			num = ((int)string[1] * 256) + num;
		}

		count += num;
		
		if(format == 2){
			num = (int)string[2];
			
		}
		else{
			num = (int)string[1];
		}
		
		
		if(((unsigned char)num) == pixel1){
			fputc(string[0],write_zip_file);
			if(format == 2){
				fputc(string[1],write_zip_file);
				fputc(pixel2,write_zip_file);
				fputc((unsigned char)'\0',write_zip_file);
			}
			else{
				fputc(pixel2,write_zip_file);
			}
			
			if(pre_index >= 0){
				if(pre_val == ((int)pixel2)){
					extra_rle[pre_index] += count;
				}
				else{
					pre_index = index;
					pre_val = pixel2;
				}
			}
			is_changed = 1;
			
			
			
		}else{
		
			fputc(string[0],write_zip_file);
			fputc(string[1],write_zip_file);
			if(format == 2){
				fputc(string[2],write_zip_file);
				fputc(string[3],write_zip_file);
			}
			
			if(is_changed){
				if(pre_val == num){
					extra_rle[pre_index] += count;
				}
				else{
					is_changed = 0;
				}
			}
			else{
				pre_index = index;
				pre_val = num;
				is_changed = 0;
			}
		
		}
		index++;
		fread(string,sizeof(unsigned char),format * 2,read_zip_file);
				
	}while(!feof(read_zip_file));
	
	fclose(read_zip_file);
	fclose(write_zip_file);
	
	remove(zip_file);

    	rename(tmp_file, zip_file);
    	
    	
    	
    	read_zip_file = fopen(zip_file,"rb");
	if(read_zip_file == NULL){
		printf("Dosya acilamadi\n");
		return;
	}
	
	write_zip_file = fopen(tmp_file,"wb");
	if(write_zip_file == NULL){
		printf("Dosya olusturulamadi\n");
		return;
	}
	
	format = (int)fgetc(read_zip_file);
	fputc((unsigned char)format,write_zip_file);
	
	fread(string,sizeof(unsigned char),format * 2,read_zip_file);
	
	fputc(string[0],write_zip_file);
	fputc(string[1],write_zip_file);
	
	if(format == 2){
		fputc(string[2],write_zip_file);
		fputc(string[3],write_zip_file);
	}
	
	fread(string,sizeof(unsigned char),format * 2,read_zip_file);
	i = 0;
	do{		
		count = 0;
		num = (int)string[0];	
		
		if(format == 2){
			num = ((int)string[1] * 256) + num;
		}

		count += num;
		
		if(format == 2){
			num = (int)string[2];
			
		}
		else{
			num = (int)string[1];
		}
		
		
		if(extra_rle[i] != 0){
			
			count += extra_rle[i];
			fputc((unsigned char)count % 256,write_zip_file);
			if(format == 2){
				fputc((unsigned char)(count >> 8),write_zip_file);
				fputc(pixel2,write_zip_file);
				fputc((unsigned char)'\0',write_zip_file);
			}
			else{
				fputc(pixel2,write_zip_file);
			}
			
		}else{
			if(pixel2 != ((unsigned char)num)){
				fputc(string[0],write_zip_file);
				fputc(string[1],write_zip_file);
				if(format == 2){
					fputc(string[2],write_zip_file);
					fputc(string[3],write_zip_file);
				}
			
			}
		
		}
		i++;
		fread(string,sizeof(unsigned char),format * 2,read_zip_file);
				
	}while(!feof(read_zip_file));
	
	fclose(read_zip_file);
	fclose(write_zip_file);
	
	remove(zip_file);

    	rename(tmp_file, zip_file);
	
	printf("%u kodlu rengi %u kodlu renkle yer degistirildi\n",pixel1,pixel2);
	
}

void editPixelWithCoordinates(char *path,char *file,unsigned char pixel,int x,int y){
	
	unsigned char string[4];
	char *zip_file,*tmp_file;
	int column,row,index,num,tmp,format,count,is_writed,size;
	
	size = strlen(path) + strlen(file);
	zip_file = (char *)malloc(size * sizeof(char));
	strcpy(zip_file,path);
	strcat(zip_file,file);
	
	FILE *read_zip_file = fopen(zip_file,"rb");
	if(read_zip_file == NULL){
		printf("Dosya acilamadi\n");
		return;
	}
	
	size = strlen(path) + 9;
	tmp_file = (char *)malloc(size * sizeof(char));
	strcpy(tmp_file,path);
	strcat(tmp_file,"file.tmp");
	
	FILE *write_zip_file = fopen(tmp_file,"wb");
	if(write_zip_file == NULL){
		printf("Dosya olusturulamad\n");
		return;
	}
	
	format = (int)fgetc(read_zip_file);
	fputc((unsigned char)format,write_zip_file);
	
	fread(string,sizeof(unsigned char),format * 2,read_zip_file);
	
	fputc(string[0],write_zip_file);
	fputc(string[1],write_zip_file);
	
	if(format == 2){
		fputc(string[2],write_zip_file);
		fputc(string[3],write_zip_file);
		column = (int)string[2];
		column = (256 * (int)string[3]) + column;
	}
	else{
		column = (int)string[1];
	}
	
	
	index = (y * column) + x;
	
	is_writed = 0;
		
	fread(string,sizeof(unsigned char),format * 2,read_zip_file);
	do{		
		num = (int)string[0];
		if(format == 2){
			num = (256 * (int)string[1]) + num;
		}
		
		if(index >= num && !is_writed){
			index -= num;
			
			fputc(string[0],write_zip_file);
			fputc(string[1],write_zip_file);
	
			if(format == 2){
				fputc(string[2],write_zip_file);
				fputc(string[3],write_zip_file);
			}
		}
		else if(!is_writed){
			if(index == 0){
			
				fputc((unsigned char)1,write_zip_file);
				if(format == 2){
					fputc((unsigned char)'\0',write_zip_file);
					fputc(pixel,write_zip_file);
					fputc((unsigned char)'\0',write_zip_file);
				}
				else{
					fputc(pixel,write_zip_file);
				}
				
				num--;
				string[0] = (unsigned char)num % 256;
				if(format == 2){
					string[1] = (unsigned char)(num >> 8);
				}
				
				fputc(string[0],write_zip_file);
				fputc(string[1],write_zip_file);
	
				if(format == 2){
					fputc(string[2],write_zip_file);
					fputc(string[3],write_zip_file);
				}
			}
			else if(index == num - 1){
				num--;
				string[0] = (unsigned char)num % 256;
				if(format == 2){
					string[1] = (unsigned char)(num >> 8);
				}
				
				fputc(string[0],write_zip_file);
				fputc(string[1],write_zip_file);
	
				if(format == 2){
					fputc(string[2],write_zip_file);
					fputc(string[3],write_zip_file);
				}
				
				
				fputc((unsigned char)1,write_zip_file);
				if(format == 2){
					fputc((unsigned char)'\0',write_zip_file);
					fputc(pixel,write_zip_file);
					fputc((unsigned char)'\0',write_zip_file);
				}
				else{
					fputc(pixel,write_zip_file);
				}
				
			}
			else{
				
				num -= index;
				string[0] = (unsigned char)index % 256;
				if(format == 2){
					string[1] = (unsigned char)(index >> 8);
				}
				
				fputc(string[0],write_zip_file);
				fputc(string[1],write_zip_file);
	
				if(format == 2){
					fputc(string[2],write_zip_file);
					fputc(string[3],write_zip_file);
				}
				
				
				fputc((unsigned char)1,write_zip_file);
				if(format == 2){
					fputc((unsigned char)'\0',write_zip_file);
					fputc(pixel,write_zip_file);
					fputc((unsigned char)'\0',write_zip_file);
				}
				else{
					fputc(pixel,write_zip_file);
				}
				
				
				num--;
				string[0] = (unsigned char)num % 256;
				if(format == 2){
					string[1] = (unsigned char)(num >> 8);
				}
				
				fputc(string[0],write_zip_file);
				fputc(string[1],write_zip_file);
	
				if(format == 2){
					fputc(string[2],write_zip_file);
					fputc(string[3],write_zip_file);
				}
				
			}
			is_writed =1;
		}
		else{
			fputc(string[0],write_zip_file);
			fputc(string[1],write_zip_file);
	
			if(format == 2){
				fputc(string[2],write_zip_file);
				fputc(string[3],write_zip_file);
			}
		}
		fread(string,sizeof(unsigned char),format * 2,read_zip_file);
			
	}while(!feof(read_zip_file));
	
	fclose(read_zip_file);
	fclose(write_zip_file);
	
	remove(zip_file);

    	rename(tmp_file, zip_file);
	
	if(is_writed){
		printf("%u kodlu renk (%d,%d)koordinati yerinde yerlestirildi\n",pixel,x,y);
	}
	else{
	
	}
	
	
}

void getHistogram(char *path,char *file){
	
	unsigned char string[4];
	int colors[MAX],i,format,count,size,num,max;
	char *zip_file;
	
	size = strlen(path);
	size += strlen(file);
	zip_file = (char *)malloc(size * sizeof(char));
	strcpy(zip_file,path);
	strcat(zip_file,file);
	
	FILE *read_zip_file = fopen(zip_file,"rb");
	if(read_zip_file == NULL){
		printf("Dosya acilamadi\n");
		return;
	}
	
	
	format = (int)fgetc(read_zip_file);
	
	eraseString((char *)colors,MAX);
	
	if(format == 2){
		count = 5;
		fseek( read_zip_file, count, SEEK_SET );
	}
	else{
		count = 3;
		fseek( read_zip_file, count, SEEK_SET );
	}
	
	fread(string,sizeof(unsigned char),format * 2,read_zip_file);
	max = 0;
	do{		
		
		num = (int)string[0];
		
		if(format == 2){
			num = ((int)string[1] * 256) + num;
		}

		count = num;
		
		if(format == 2){
			num = (int)string[2];
		}
		else{
			num = (int)string[1];
		}
		
		if(max < num){
			max = num;
		}
		
		colors[num] += count;
					
		fread(string,sizeof(unsigned char),format * 2,read_zip_file);
			
	}while(!feof(read_zip_file));
	
	i = 0;
	printf("\n-----Renk histogramı---\n");
	while(i <= max){
		if(colors[i] != 0){
			printf("%d renk kodu %d defa kullanilmistir\n",i,colors[i]);
		}
		i++;
	}
	
}

int main(){
	
	int option,option1,i,j,k,size,exit,x,y,pixel1,pixel2;
	char path[FILENAME_MAX],file[FILENAME_MAX],tmp[FILENAME_MAX];
	

	printf("----PGM dosyası üzerinden RLE metoduyla ziplyip uzerinden pixel editlem yapılmaktadır---\n---Parantez içindeki sayılardan birini girerek islemleri yapabilirsiniz----\n---Dosya yolunun ornek format girdisi: C:\\to\\the\\directory\\ veya /to/the/directory/\n---Ziplenen ve zipten cikarma islemleri girilen dosya yolundaki klasorde icermektedir---\n");
	exit = 0;
	while(!exit){
		printf("\nYapmak istediğiniz işlemlerden birini seçiniz\n");
		printf("\nPgm görsel dosyasını okuma(1)\nPgm dosyasini zipleme(2)\nZiplenmis dosyanın kontrol etme(3)\nZip dosyasını acma(4)\nPixel editleme(5)\nCikis(6)\n");
		scanf("%d",&option);
		switch(option){
			case 1:{
				printf("\nPgm dosyasının yolunu giriniz :");
				scanf("%s",path);
				printf("\nPgm dosyasının ismini giriniz:");
				scanf("%s",file);  
				
				strcat(path,file);
				getPGMMatrix(path);
			
				printf("\nwidth:%d,height:%d\n",width,height);
				printf("max value:%d\n",max_value);
				for(i = 0;i < height;i++){
					for(j = 0;j < width;j++){
						printf("%d(%d,%d) ",pgm_image_matrix[i][j],i,j);
					}
					printf("\n");
				}
			} 
			break;
			case 2:{
				printf("\nPgm dosyasının yolunu giriniz :");
				scanf("%s",path);
				printf("\nPgm dosyasının ismini giriniz:");
				scanf("%s",file);  
				
				strcpy(tmp,path);
				strcat(path,file);
				getPGMMatrix(path);
				zipPgmWithRLE(tmp,file);
			}
			break;
			case 3:{
				printf("\nZip dosyasının yolunu giriniz :");
				scanf("%s",path);
				
				printf("\nZip dosyası hangi isim üzerinden işlensin ?\ntest_encoded.txt(1),ozel isim(2)\n");
				scanf("%d",&option1);
				
				if(option1 == 2){
					printf("\nZip dosyasının ismini giriniz:");
					scanf("%s",file); 
				}else{
					strcpy(file,"test_encoded.txt");
				}
				
				strcat(path,file);
				controlZipFile(path);
			}
			break;
			case 4: {
				printf("\nZip dosyasının yolunu giriniz :");
				scanf("%s",path);
				
				printf("\nZip dosyası hangi isim üzerinden işlensin ?\ntest_encoded.txt(1),ozel isim(2)\n");
				scanf("%d",&option1);
				
				if(option1 == 2){
					printf("\nZip dosyasının ismini giriniz:");
					scanf("%s",file); 
				}else if(option1 == 1){
					strcpy(file,"test_encoded.txt");
				}
				else{
					printf("\nGecersiz girdi\n");
				}
				
				extractZipPGM(path,file);
			}
			break;
			case 5: {
				printf("\nZip dosyasının yolunu giriniz:");
				scanf("%s",path);
				
				printf("\nZip dosyası hangi isim üzerinden işlensin ?\ntest_encoded.txt(1),ozel isim(2)\n");
				scanf("%d",&option1);
				
				if(option1 == 2){
					printf("\nZip dosyasının ismini giriniz:");
					scanf("%s",file); 
				}else if(option1 == 1){
					strcpy(file,"test_encoded.txt");
				}
				else{
					printf("\nGecersiz girdi\n");
				}
				
				printf("\nHangi edit islemi yapilacak?\nPixel degeri degistirme(1),Koordinatlarla bir pixel degistirme(2),Histogram alma(3)\n");
				scanf("%d",&option1);
				
				if(option1 == 1){
					printf("\nAradiginiz pixel degeri giriniz (0 ile 255 arasinda giriniz):");
					scanf("%d",&pixel1);
					
					if(pixel1 < 0 || 255 < pixel1){
						printf("\nGerekli girdi renk degerini gitmediniz\n");
						break;
					}
					
					printf("\nYeni pixel degeri giriniz (0 ile 255 arasinda giriniz):");
					scanf("%d",&pixel2);
					
					if(pixel2 < 0 || 255 < pixel2){
						printf("\nGerekli girdi renk degerini gitmediniz\n");
						break;
					}
					
					editPixels(path,file,(unsigned char)pixel1,(unsigned char)pixel2);
				}
				else if(option1 == 2){
					printf("\nYeni pixel degeri giriniz (0 ile 255 arasinda giriniz):");
					scanf("%d",&pixel1);
					
					if(pixel1 < 0 || 255 < pixel1){
						printf("\nGerekli girdi renk degerini gitmediniz\n");
						break;
					}
					
					printf("\nSatir degeri giriniz :");
					scanf("%d",&y);
					
					printf("\nSutun degeri giriniz:");
					scanf("%d",&x);
					
					editPixelWithCoordinates(path,file,(unsigned char)pixel1,x,y);
				}
				else if(option == 3){
					getHistogram(path,file);
				}
				else{
					printf("\nGecersiz girdi\n");
				}
			
			}
			break;
			case 6: {
				exit = 1;
				printf("\nİslemden cıktınız\n");
			}
			break;
			default:{
				printf("\nGecersiz girdi\n");
			}
			break;
		}	
	}
	
	
	return 0;
}
