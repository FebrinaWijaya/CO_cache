#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>

#define DIRECT_MAPPED 0
#define FOUR_WAY_SET_ASSOCIATIVE 1
#define FULLY_ASSOCIATIVE 2

#define FIFO 0
#define LRU 1

 typedef struct Cache_DirectMapped
 {
 	bool valid;
 	int tag;
 	int data;
 }Cache_DirectMapped;

 typedef struct Cache_4WaySetAssociative
 {
 	bool valid[4];
 	int tag[4];
 	int data[4];
 	int out_list[4];
 }Cache_4WaySetAssociative;

 typedef struct Cache_FullyAssociative
 {
	bool valid;
 	int tag;
 	int data;
 }Cache_FullyAssociative;
 
 typedef struct Node 
{
  int data;
  struct Node *next;
}Node;

 int main(int argc,char *argv[])
 {
	if(argc<5) printf("Not enough argument");
	char *input,*output;
		input=argv[2];
		output=argv[4];
 	FILE *file_in,*file_out;
 	file_in=fopen(input,"r");
	if(file_in==NULL)
	{
		printf("Failed to open input file %s\n",input);
		return 0;
	}
	file_out=fopen(output,"w"); 
	if(file_in==NULL)
	{
		printf("Failed to open output file %s\n",output);
		return 0;
	}
	int cache_size,block_size,associativity,replace_algo;
	fscanf(file_in,"%d",&cache_size);
	fscanf(file_in,"%d",&block_size);
	fscanf(file_in,"%d",&associativity);
	fscanf(file_in,"%d",&replace_algo);
	
	int num_of_blocks=cache_size*1024/block_size;
	
	Node *hit_first=NULL,*hit_last=NULL;
	Node *miss_first=NULL,*miss_last=NULL;
	if(associativity==DIRECT_MAPPED)
	{
		Cache_DirectMapped cache[num_of_blocks];
		int index=0;
		for(index=0;index<num_of_blocks;index++)
		{
			cache[index].valid=false;
			cache[index].tag=0;
			cache[index].data=0; 
		}
		unsigned int addr;
		int i=0;
		while(fscanf(file_in,"%x",&addr)==1)
		{
			++i;
			index=(addr/block_size)%num_of_blocks;
			int tag=addr/block_size/num_of_blocks;
			if(cache[index].valid&&cache[index].tag==tag)
			{
				//record hit instruction
				if(hit_last==NULL) 
				{
					hit_first=(Node*)malloc(sizeof(Node));
					hit_first->data=i;
					hit_first->next=NULL;
					hit_last=hit_first;
				}
				else
				{
					hit_last->next=(Node*)malloc(sizeof(Node));
					hit_last=hit_last->next;
					hit_last->data=i;
					hit_last->next=NULL;
				}
			}
			else
			{
				//record miss instruction
				if(miss_last==NULL) 
				{
					miss_first=(Node*)malloc(sizeof(Node));
					miss_first->data=i;
					miss_first->next=NULL;
					miss_last=miss_first;
				}
				else
				{
					miss_last->next=(Node*)malloc(sizeof(Node));
					miss_last=miss_last->next;
					miss_last->data=i;
					miss_last->next=NULL;
				}
				cache[index].valid=true;
				cache[index].tag=tag;
			}
		}
		
	}
	else if(associativity==FOUR_WAY_SET_ASSOCIATIVE)
	{
		int num_of_sets=num_of_blocks/4;
		Cache_4WaySetAssociative cache[num_of_sets];
		int i;
		for(i=0;i<num_of_sets;i++)
		{
			int j;
			for(j=0;j<4;j++)
			{
				cache[i].valid[j]=false;
				cache[i].tag[j]=0;
				cache[i].data[j]=0;
				cache[i].out_list[j]=j;
			}
		} 
		unsigned int addr;
		i=0;
		while(fscanf(file_in,"%x",&addr)==1)
		{
			++i;
			int index=(addr/block_size)%num_of_sets;
			int tag=addr/block_size/num_of_sets;
			int j;
			bool found=false;
			for(j=0;j<4;j++)
				if(cache[index].valid[j]&&cache[index].tag[j]==tag)
				{
					found=true;
					//record hit instruction
					if(hit_last==NULL) 
					{
						hit_first=(Node*)malloc(sizeof(Node));
						hit_first->data=i;
						hit_first->next=NULL;
						hit_last=hit_first;
					}
					else
					{
						hit_last->next=(Node*)malloc(sizeof(Node));
						hit_last=hit_last->next;
						hit_last->data=i;
						hit_last->next=NULL;
					}
					if(replace_algo==LRU) 
					{
						int k;
						for(k=0;k<4;k++)
						if(cache[index].out_list[k]==j)
						{
							int l;
							for(l=k;l<3;l++)
								cache[index].out_list[l]=cache[index].out_list[l+1];
							cache[index].out_list[3]=j;
							break;
						}
					}
					break;
				}
			if(!found)
			{
				//record miss instruction
				if(miss_last==NULL) 
				{
					miss_first=(Node*)malloc(sizeof(Node));
					miss_first->data=i;
					miss_first->next=NULL;
					miss_last=miss_first;
				}
				else
				{
					miss_last->next=(Node*)malloc(sizeof(Node));
					miss_last=miss_last->next;
					miss_last->data=i;
					miss_last->next=NULL;
				}
				int j = cache[index].out_list[0];
				int k;
				for(k=0;k<3;k++)
					cache[index].out_list[k]=cache[index].out_list[k+1];;
				cache[index].out_list[3]=j;
				cache[index].valid[j]=true;
				cache[index].tag[j]=tag;
			}
		}
	}
	else if(associativity==FULLY_ASSOCIATIVE)
	{
		Cache_FullyAssociative cache[num_of_blocks];
		int out_list[num_of_blocks];
		int index=0;
		for(index=0;index<num_of_blocks;index++)
		{
			cache[index].valid=false;
			cache[index].tag=0;
			cache[index].data=0; 
			out_list[index]=index;
		}
		unsigned int addr;
		int i=0;
		while(fscanf(file_in,"%x",&addr)==1)
		{
			++i;
			int tag=addr/block_size;
			bool found=false;
			for(index=0;index<num_of_blocks;index++)
				if(cache[index].valid&&cache[index].tag==tag)
				{
					found=true;
					//record hit instruction
					if(hit_last==NULL) 
					{
						hit_first=(Node*)malloc(sizeof(Node));
						hit_first->data=i;
						hit_first->next=NULL;
						hit_last=hit_first;
					}
					else
					{
						hit_last->next=(Node*)malloc(sizeof(Node));
						hit_last=hit_last->next;
						hit_last->data=i;
						hit_last->next=NULL;
					}
					
					if(replace_algo==LRU) 
					{
						int k;
						for(k=0;k<num_of_blocks;k++)
						if(out_list[k]==index)
						{
							int l;
							for(l=k;l<num_of_blocks-1;l++)
								out_list[l]=out_list[l+1];
							out_list[num_of_blocks-1]=index;
							break;
						}
					}
					break;
				}
			if(!found)
			{
				//record miss instruction
				if(miss_last==NULL) 
				{
					miss_first=(Node*)malloc(sizeof(Node));
					miss_first->data=i;
					miss_first->next=NULL;
					miss_last=miss_first;
				}
				else
				{
					miss_last->next=(Node*)malloc(sizeof(Node));
					miss_last=miss_last->next;
					miss_last->data=i;
					miss_last->next=NULL;
				}
				index=out_list[0];
				int j;
				for(j=0;j<num_of_blocks-1;j++)
					out_list[j]=out_list[j+1];
				out_list[num_of_blocks-1]=index;
				cache[index].valid=true;
				cache[index].tag=tag;
			}
		}
	}
	int miss=0,hit=0;
	fprintf(file_out,"Hits instructions:");
	hit_last=hit_first;
	while(hit_last!=NULL)
	{
		++hit;
		fprintf(file_out," %d",hit_last->data);
		hit_last=hit_last->next;
	}
	fprintf(file_out,"\nMisses instructions:");
	miss_last=miss_first;
	while(miss_last!=NULL)
	{
		++miss;
		fprintf(file_out," %d",miss_last->data);
		miss_last=miss_last->next;
	}
	fprintf(file_out,"\nMiss rate: %f\n",(float)miss/(miss+hit));	
 	fclose(file_in);
 	fclose(file_out);
 }
 
