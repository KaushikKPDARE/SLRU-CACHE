#include <stdio.h>
#include <stdlib.h>
int hit =0, miss=0;
typedef struct Node
{
struct Node *prev, *next;
unsigned block;
} Node;
typedef struct Hash
{
int hashSize;
Node* *array;
} Hash;
typedef struct Cache
{
unsigned count;
unsigned cacheSize;
Node *front, *rear;
} Cache;
Node* newNode( unsigned block )
{
Node* temp = (Node *)malloc( sizeof( Node ) );
temp->block = block;
temp->prev = temp->next = NULL;
return temp;
}
Cache* createCache( int cacheSize )
{
Cache* cache = (Cache *)malloc( sizeof( Cache ) );
cache->count = 0;
cache->front = cache->rear = NULL;
cache->cacheSize = cacheSize;
return cache;
}
Hash* createHash( int hashSize )
{
Hash* hash = (Hash *) malloc( sizeof( Hash ) );
hash->hashSize = hashSize;
hash->array = (Node **) malloc( hash->hashSize * sizeof( Node* ) );
int i;
for( i = 0; i < hash->hashSize; ++i )
hash->array[i] = NULL;
return hash;
}
int CacheFull( Cache* cache )
{
return cache->count == cache->cacheSize;
}
int cacheEmpty( Cache* cache )
{
return cache->rear == NULL;
}
void unlinkCache( Cache* cache )
{
if( cacheEmpty( cache ) )
return;
if (cache->front == cache->rear)
cache->front = NULL;
Node* temp = cache->rear;
cache->rear = cache->rear->prev;
if (cache->rear)
cache->rear->next = NULL;
free( temp );
cache->count--;
}
void LinkCache1( Cache* cache, Hash* hash, unsigned block, Node* temp )
{
if ( CacheFull ( cache ) )
{
hash->array[ cache->rear->block ] = NULL;
unlinkCache( cache );
}
temp->next = cache->front;
if ( cacheEmpty( cache ) )
cache->rear = cache->front = temp;
else
{
cache->front->prev = temp;
cache->front = temp;
}
hash->array[ block ] = temp;
cache->count++;
}
void unlinkCache1( Cache* cache, Cache* cache1, Hash* hash, unsigned block )
{
if( cacheEmpty( cache ) )
return;
if (cache->front == cache->rear)
cache->front = NULL;
Node* temp = cache->rear;
cache->rear = cache->rear->prev;
if (cache->rear)
cache->rear->next = NULL;
LinkCache1(cache1, hash,block, temp);
cache->count--;
}
void LinkCache( Cache* cache, Hash* hash, unsigned block, Cache* cache1 )
{
if ( CacheFull ( cache ) )
{
unlinkCache1( cache, cache1, hash, block );
}
Node* temp = newNode( block );
temp->next = cache->front;
if ( cacheEmpty( cache ) )
cache->rear = cache->front = temp;
else
{
cache->front->prev = temp;
cache->front = temp;
}
hash->array[ block ] = temp;
cache->count++;
}
void SearchBlock( Cache* cache, Hash* hash, unsigned block, Cache* cache1 )
{
Node* findBlock = hash->array[ block ];
if ( findBlock == NULL ){
miss++;
LinkCache( cache, hash, block, cache1 );
}
else if (findBlock != cache->front)
{
findBlock->prev->next = findBlock->next;
if (findBlock->next)
findBlock->next->prev = findBlock->prev;
if (findBlock == cache->rear)
{
cache->rear = findBlock->prev;
cache->rear->next = NULL;
}
findBlock->next = cache->front;
findBlock->prev = NULL;
findBlock->next->prev = findBlock;
cache->front = findBlock;
if(cache->front == findBlock)
hit++;
}
else if (findBlock == cache->front)
hit++;
}
int main()
{
Cache* ca = createCache( 256 );
Cache* ca1 = createCache(256 );
Hash* hash = createHash( 1000000 );
FILE* fp=NULL;
fp=fopen("filekau.txt","r");
int i;
while(fscanf(fp, "%d", &i) != EOF){
SearchBlock( ca, hash, i, ca1);
}
fclose(fp);
printf("miss ratio=%f \nhit ratio=%f \n",((float)miss/(hit+miss)),((float)hit/(hit+miss)));
return 0;
}
