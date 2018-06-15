" 編碼
set encoding=utf-8
set fileencodings=utf-8,cp950

" 語法上色顯示
syntax on
" 自動縮排
set ai          
" 設定縮排寬度4 
set shiftwidth=4
" tab 鍵改為4個空白
set tabstop=4   
" 顯示右下角參數,如第幾行第幾個字
set ruler        
" 在 insert 也可用 backspace
set backspace=2  
" 設定搜尋忽略大小寫
set ic   
" 設定高亮度顯示搜尋結果        
set hlsearch  
" 在關鍵字還沒完全輸入完畢前就顯示結果   
set incsearch    
" 設定 smartindent
set smartindent  
" 操作過程有衝突時，以明確的文字來詢問
set confirm      
" 保留 200 個使用過的指令
set history=200  
" 顯示目前的游標位置
set cursorline   
" 顯示行數
set number       
" 搜尋不會有底色
:nohl            
" 字串太長不自動換行
:set nowrap      
" 個人喜好顏色配置
colorscheme torte
" 啟用游標選取：游標可以直接選取文字，滾輪可以直接滑動頁面 ( 非移動游標 )。
:set mouse=a