
#include "merkleTreeBN128.hpp"
#include <algorithm> // std::max
#include "zkassert.hpp"

MerkleTreeBN128::MerkleTreeBN128(uint64_t _height, uint64_t _width)
{
    source = (Goldilocks::Element *)malloc(_height * _width * sizeof(Goldilocks::Element));
    if( source == NULL){
        std::cout << "Error: MerkleTreeBN128() failed allocating memory size: " << _height * _width * sizeof(Goldilocks::Element) << std::endl;
        exitProcess();
    }
    
    source_width = _width;
    isSourceAllocated = true;
    height = _height;
    (_width > GOLDILOCKS_ELEMENTS + 1) ? width = ceil((double)_width / GOLDILOCKS_ELEMENTS) : width = 0;
    numNodes = getNumNodes(height);
    nodes = (RawFr::Element *)calloc(numNodes, sizeof(RawFr::Element));
    if( nodes == NULL){
        std::cout << "Error: MerkleTreeBN128() failed allocating memory size: " << numNodes * sizeof(RawFr::Element) << std::endl;
        exitProcess();
    }
    isNodesAllocated = true;
}

void MerkleTreeBN128::initialize(Goldilocks::Element *_source)
{
    std::memcpy(source, _source, height * source_width * sizeof(Goldilocks::Element));
    merkelize();
    intialized = true;
}

MerkleTreeBN128::MerkleTreeBN128(uint64_t _height, uint64_t _width, Goldilocks::Element *_source) : source(_source), height(_height), width(_width), source_width(_width)
{

    if (source == NULL)
    {
        source = (Goldilocks::Element *)calloc(height * width, sizeof(Goldilocks::Element));
        if(source == NULL){
            std::cout << "Error: MerkleTreeBN128() failed allocating memory size: " << height * width * sizeof(Goldilocks::Element) << std::endl;
            exitProcess();
        }
        isSourceAllocated = true;
    }

    (_width > GOLDILOCKS_ELEMENTS + 1) ? width = ceil((double)_width / GOLDILOCKS_ELEMENTS) : width = 0;
    numNodes = getNumNodes(height);
    nodes = (RawFr::Element *)calloc(numNodes, sizeof(RawFr::Element));
    if (nodes == NULL)
    {
        std::cout << "Error: MerkleTreeBN128() failed allocating memory size: " << numNodes * sizeof(RawFr::Element) << std::endl;
        exitProcess();
    }
    isNodesAllocated = true;
    intialized = true;
}

MerkleTreeBN128::MerkleTreeBN128(void *_source)
{
    Goldilocks::Element *tree = (Goldilocks::Element *)_source;
    source_width = Goldilocks::toU64(tree[0]);
    height = Goldilocks::toU64(tree[1]);
    source = &tree[2];
    numNodes = getNumNodes(height);

    nodes = (RawFr::Element *)&source[source_width * height];
}

MerkleTreeBN128::~MerkleTreeBN128()
{
    if (isNodesAllocated)
    {
        free(nodes);
    }
    if (isSourceAllocated)
    {
        free(source);
    }
}

uint64_t MerkleTreeBN128::getNumNodes(uint64_t n)
{
    uint n_tmp = n;
    uint64_t nextN = floor(((double)(n_tmp - 1) / MT_BN128_ARITY) + 1);
    uint64_t acc = nextN * MT_BN128_ARITY;
    while (n_tmp > 1)
    {
        // FIll with zeros if n nodes in the leve is not even
        n_tmp = nextN;
        nextN = floor((n_tmp - 1) / MT_BN128_ARITY) + 1;
        if (n_tmp > 1)
        {
            acc += nextN * MT_BN128_ARITY;
        }
        else
        {
            acc += 1;
        }
    }

    return acc;
}

/*
 * LinearHash BN128
 */
void MerkleTreeBN128::linearHash()
{
    if (source_width > 4)
    {
        uint64_t widthRawFrElements = ceil((double)source_width / GOLDILOCKS_ELEMENTS);
        RawFr::Element *buff = (RawFr::Element *)calloc(height * widthRawFrElements, sizeof(RawFr::Element));
        if(buff == NULL){
            std::cout << "Error: linearHash() failed allocating memory size: " << height * widthRawFrElements * sizeof(RawFr::Element) << std::endl;
            exitProcess();
        }

#pragma omp parallel for
        for (uint64_t i = 0; i < height; i++)
        {
            for (uint64_t j = 0; j < width; j++)
            {
                uint64_t pending = source_width - j * GOLDILOCKS_ELEMENTS;
                uint64_t batch;
                (pending >= GOLDILOCKS_ELEMENTS) ? batch = GOLDILOCKS_ELEMENTS : batch = pending;
                for (uint64_t k = 0; k < batch; k++)
                {
                    buff[i * width + j].v[k] = Goldilocks::toU64(source[i * source_width + j * GOLDILOCKS_ELEMENTS + k]);
                }
                RawFr::field.toMontgomery(buff[i * width + j], buff[i * width + j]);
            }
        }

#pragma omp parallel for
        for (uint64_t i = 0; i < height; i++)
        {
            uint pending = width;
            Poseidon_opt p;
            std::vector<RawFr::Element> elements(MT_BN128_ARITY + 1);
            while (pending > 0)
            {
                std::memset(&elements[0], 0, (MT_BN128_ARITY + 1) * sizeof(RawFr::Element));
                if (pending >= MT_BN128_ARITY)
                {
                    std::memcpy(&elements[1], &buff[i * width + width - pending], MT_BN128_ARITY * sizeof(RawFr::Element));
                    std::memcpy(&elements[0], &nodes[i], sizeof(RawFr::Element));
                    p.hash(elements, &nodes[i]);
                    pending = pending - MT_BN128_ARITY;
                }
                else
                {
                    std::vector<RawFr::Element> elements_last(pending + 1);
                    std::memcpy(&elements_last[1], &buff[i * width + width - pending], pending * sizeof(RawFr::Element));
                    std::memcpy(&elements_last[0], &nodes[i], sizeof(RawFr::Element));
                    p.hash(elements_last, &nodes[i]);
                    pending = 0;
                }
            }
        }
        free(buff);
    }
    else
    {
#pragma omp parallel for
        for (uint64_t i = 0; i < height; i++)
        {
            for (uint64_t k = 0; k < source_width; k++)
            {
                nodes[i].v[k] = Goldilocks::toU64(source[i * source_width + k]);
            }
            RawFr::field.toMontgomery(nodes[i], nodes[i]);
        }
    }
}

void MerkleTreeBN128::merkelize()
{

    linearHash();

    RawFr::Element *cursor = &nodes[0];
    uint64_t n256 = height;
    uint64_t nextN256 = floor((double)(n256 - 1) / MT_BN128_ARITY) + 1;
    RawFr::Element *cursorNext = &nodes[nextN256 * MT_BN128_ARITY];
    while (n256 > 1)
    {
        uint64_t batches = ceil((double)n256 / MT_BN128_ARITY);
#pragma omp parallel for
        for (uint64_t i = 0; i < batches; i++)
        {
            Poseidon_opt p;
            vector<RawFr::Element> elements(MT_BN128_ARITY + 1);
            std::memset(&elements[0], 0, (MT_BN128_ARITY + 1) * sizeof(RawFr::Element));
            uint numHashes = (i == batches - 1) ? n256 - i*MT_BN128_ARITY : MT_BN128_ARITY;
            std::memcpy(&elements[1], &cursor[i * MT_BN128_ARITY], numHashes * sizeof(RawFr::Element));
            p.hash(elements, &cursorNext[i]);
        }

        n256 = nextN256;
        nextN256 = floor((double)(n256 - 1) / MT_BN128_ARITY) + 1;
        cursor = cursorNext;
        cursorNext = &cursor[nextN256 * MT_BN128_ARITY];
    }
}

void MerkleTreeBN128::getRoot(RawFr::Element *root)
{
    std::memcpy(root, &nodes[numNodes - 1], sizeof(RawFr::Element));
}

uint64_t MerkleTreeBN128::getMerkleProofLength(uint64_t n)
{
    return ceil((double)log(n) / log(MT_BN128_ARITY));
}

uint64_t MerkleTreeBN128::getMerkleProofSize(uint64_t n)
{
    return getMerkleProofLength(n) * MT_BN128_ARITY * sizeof(RawFr::Element);
}

void MerkleTreeBN128::getGroupProof(void *res, uint64_t idx)
{
    assert(idx < height);

    Goldilocks::Element v[source_width];
    for (uint64_t i = 0; i < source_width; i++)
    {
        v[i] = getElement(idx, i);
    }
    std::memcpy(res, &v[0], source_width * sizeof(Goldilocks::Element));
    void *resCursor = (uint8_t *)res + source_width * sizeof(Goldilocks::Element);

    RawFr::Element *mp = (RawFr::Element *)calloc(getMerkleProofSize(height), 1);
    if(mp==NULL){
        std::cout << "Error: getGroupProof() failed allocating memory size: " << getMerkleProofSize(height) << std::endl;
        exitProcess();
    }
    merkle_genMerkleProof(mp, idx, 0, height);

    std::memcpy(resCursor, &mp[0], getMerkleProofSize(height));
    free(mp);
}

Goldilocks::Element MerkleTreeBN128::getElement(uint64_t idx, uint64_t subIdx)
{
    return source[source_width * idx + subIdx];
}

void MerkleTreeBN128::merkle_genMerkleProof(RawFr::Element *proof, uint64_t idx, uint64_t offset, uint64_t n)
{
    if (n <= 1)
        return;
    
    uint64_t nBitsArity = std::ceil(std::log2(MT_BN128_ARITY));

    uint64_t nextIdx = idx >> nBitsArity;
    uint64_t si = idx ^ (idx & (MT_BN128_ARITY - 1));

    std::memcpy(proof, &nodes[offset + si], MT_BN128_ARITY * sizeof(RawFr::Element));
    uint64_t nextN = (std::floor((n - 1) / MT_BN128_ARITY) + 1);
    merkle_genMerkleProof(&proof[MT_BN128_ARITY], nextIdx, offset + nextN * MT_BN128_ARITY, nextN);
}
