# Simple Shell - Flowchart Complet
```mermaid
graph TD
    A[START]
    B[Initialize variables]
    C[Loop while 1]
    D{isatty check}
    E[Print prompt]
    F[No prompt]
    G[getline read]
    H{EOF check}
    I[Exit free memory]
    J[increment line_count]
    K[parse_line tokenize]
    L{argv is NULL}
    M[run_command]
    N{is builtin}
    O[execute builtin]
    P[find command PATH]
    Q{Command found}
    R[Error 127 not found]
    S[spawn and wait]
    T[fork process]
    U{pid value}
    V[execve child]
    W[waitpid parent]
    X[get exit status]
    Y[save status]
    Z[free continue loop]
    END[END]
    
    A --> B
    B --> C
    C --> D
    D -->|Yes| E
    D -->|No| F
    E --> G
    F --> G
    G --> H
    H -->|Yes| I
    H -->|No| J
    J --> K
    K --> L
    L -->|Yes| C
    L -->|No| M
    M --> N
    N -->|Yes| O
    N -->|No| P
    P --> Q
    Q -->|No| R
    Q -->|Yes| S
    S --> T
    T --> U
    U -->|Child| V
    U -->|Parent| W
    V --> X
    W --> X
    X --> Y
    O --> Y
    R --> Y
    Y --> Z
    Z --> C
    I --> END
