#pragma once
#include<cstdint>

namespace AcornEngine{

    struct BlockHeader{
        BlockHeader* pNext;
    };

    struct PageHeader {
        PageHeader* pNext;
        BlockHeader* Blocks() {
                return reinterpret_cast<BlockHeader*>(this + 1);
        }
    };

    class Allocator{
    public:
        Allocator(size_t DataSize = 4, size_t PageSize = 128, size_t Alignment = 4);
        ~Allocator();
        Allocator(const Allocator& clone) = delete;
        Allocator &operator=(const Allocator &rhs) = delete;

        void Reset(size_t data_size, size_t page_size, size_t alignment);

        void* Allocate();
        void  Free(void* p);
        void  FreeAll();

        static const uint8_t PATTERN_ALIGN = 0xFC;
        static const uint8_t PATTERN_ALLOC = 0xFD;
        static const uint8_t PATTERN_FREE  = 0xFE;

    private:
#if defined(_DEBUG)
        // fill a free page with debug patterns
        void FillFreePage(PageHeader* pPage);

        // fill a block with debug patterns
        void FillFreeBlock(BlockHeader* pBlock);

        // fill an allocated block with debug patterns
        void FillAllocatedBlock(BlockHeader* pBlock);
#endif

        static inline size_t CalcAlignment(size_t DataSize, size_t Alignment){
            return (DataSize+Alignment-1) & ~(Alignment-1);
        }

        BlockHeader* NextBlock(BlockHeader* pBlock);

        PageHeader* m_pPageList;

        BlockHeader* m_pFreeList;

        size_t      m_szDataSize;
        size_t      m_szPageSize;
        size_t      m_szAlignmentSize;
        size_t      m_szBlockSize;
        uint32_t    m_nBlocksPerPage;

        uint32_t    m_nPages;
        uint32_t    m_nBlocks;
        uint32_t    m_nFreeBlocks;

    };

}