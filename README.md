# Physical Cache Simulator
Program to simulate processor cache operations.


# Functions

### Mapping Functions:

1. Direct Mapping
2. Full-Associative Mapping
3. Set-Associative Mapping

### Replacement Algorithms:

1. FIFO
2. LFU
3. LRU
4. Random

### Write-Hit Policies:

1. Write-Trough
2. Write-Back

### Write-Miss Policies:

1. Write-Allocate
2. No-Write-Allocate

### Cache Array Segmentation:

1. Cache[x][0]: Valid indicator
2. Cache[x][1]: Tag
3. Cache[x][2]: Replacement Algorithm Counter
4. Cache[x][3]:  Dirty-bit
