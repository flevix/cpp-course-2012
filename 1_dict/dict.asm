;Выполнил: Нечаев Михаил
;Группа: 1538
;Вариант: 4
;-------------------------------------------------------
;длина ключа и значения -- 31 символ
;размер словаря -- 64 пары <ключ, значение>
;Created in "FASM Editor 2.0"
;Compiled by "flat assembler  version 1.69.50"
;OS: Windows 7, x86
;362 строчки кода -- Sad but true.
      
	use16		;генерировать 16-битный код
	org 100h	;программа начинается с адреса 100h
	jmp start_programm	
;---объявление переменных-------------------------------
press	: db 13,10,'Press any key for close application..$'
error   : db 13,10,'Error. Please, restart application. Test was incorrect. =($'
ent : db 13,10,'$'  ;код перевода строки
len_cell dw 20h ;размер одной ячейки
left dw ?   ;переменные для удобства
right dw ?  ;все переменные типа word, чтобы не приходилось приводить к типу регистра
medium dw ? ;хотя, на самом деле, это совсем не обязательно
flag dw ?   ;переменная для хранения результата сравнения строк
arr_length dw ?         ;arrrrrr.. Я надеваю свой плащ и волшебную шляпу..
key         : rb 32
arr_keys	: rb 2048	;длина строки 31 бит, всего 64 значения
arr_values	: rb 2048	;аналогично
arr_tmp_keys	: rb 2048	;массив хранения отсортированных последовательностей ключей
arr_tmp_values	: rb 2048	;-//- значений	
;---код программы---------------------------------------
start_programm:
    xor bx, bx  ;временное хранeние позиции текущей ячейки (изначально == 0)
	mov di, arr_keys ;записываем адрес массива для ключей, далее аналогично 
	jmp read_key1  ;сразу переходим на метку чтения, потому запись производится с нуля
    
next_read_init: ;выполнение промежуточных операций до чтения ключа
    mov ah, 09h     ;Функция DOS 09h - вывод строки
    mov dx, ent   ;в DX адрес строки
    int 21h     ;вывод переноса строки, чтобы в консоли не затирались буквы
    
    mov [di], byte('$') ;добавляем к строке value знак конца строки
    add bx, [len_cell]  ;увеличиваем индекс ячейки
    mov di, arr_keys    ;записываем в 'di' адрес массива для ключей
    add di, bx          ;переходим на следующую условную ячейку массива        
    
read_key1:  ;чтение ключа
    mov ah, 01h ;функция DOS 01h - ввод символа, который помещается в 'al'
    int 21h
    cmp al, 13  ;'13' код перевода строки
        je pre_merge    ;если считанный символ равен enter, то выходим из чтения и идем сортировать
    cmp al, 32 ;'32' код пробела
        je next_value_init  ;если пробел, начинаем читать значениеы
    mov [di], al    ;записываем считанную букву строку для ключа
    inc di      ;переходим на следующий байт
    jmp read_key1   ;считываем пока не пробел или перевод строки

next_value_init:    ;выполнение промежуточных операций до чтения значения
    mov [di], byte('$') ;добавляем знак конца строки к строке key
    mov di, arr_values  ;записываем в 'di' ядрес массива для значений
    add di, bx  ;переходим на следующую ячейку массива

read_value1:    ;считывание строки значения
    mov ah, 01h 
    int 21h ;считали символ        
    cmp al, 13  ;'13' код перевода строки
        je next_read_init   ;если enter -- возвращаемся к чтению ключей
    mov [di], al    ;записали считанный символ строки для значения
    inc di  ;перешли на следующий байт
    jmp read_value1 ;считываем, пока не будет enter                    

pre_merge:  ;выполнение промежуточных операций до сортировки
    shr bx, 5   ;уменьшаем 'bx' в 32 раза, чтобы узнать количество пар key<->value  
    cmp bx, 0   ;если в 'bx' теперь 0, значит ввод был пуст
        je error_exit   ;поэтому выведим сообщение об ошибке                            	   
    mov [arr_length], bx    ;запишем в переменную "arr_length" количество элементов
    dec bx  ;уменьшим 'bx' так как индексация с нуля и теперь bx - right
    mov cx, bx  ;cx - right
    xor ax, ax  ;'ax' - left
    call sort ;вызов процедуры сортировки (отсортируем массивы)
;-------------------------------------------------------------------
pre_read_key2:
    mov ah, 09h     ;Функция DOS 09h - вывод строки
    mov dx, ent   ;в DX адрес строки
    int 21h     ;вывод переноса строки, чтобы в консоли не затирались буквы
    
    mov ah, 01h 
    int 21h
    cmp al, 13  ;если опять enter, значит ввод тестов закончился. выходим                                                              
        je no_error_exit_programm
    mov di, key ;иначе записываем считанный символ в вводимый клюй
    mov [di], al
    inc di
    
read_key2:
    mov ah, 01h
    int 21h
    cmp al, 13  ;если enter, то ключ введён, идем искать значение
        je pre_binary_search
    mov [di], al    ;иначе добавляем символ к ключу
    inc di
    jmp read_key2

pre_binary_search:
    mov [di], byte('$') ;добавляем конец строки к ключу
    mov ax, 0   ;ax - left
    mov cx, [arr_length]    
    dec cx  ;cx - right
        
pre_binary_search2:
    cmp ax, cx
        jg error_exit   ;если left > right значит такого ключа нет -> error
    mov di, key ;second string
    mov si, arr_keys    ;first string
    mov bx, cx  ;вычисляем медиану  
    add bx, ax
    shr bx, 1
    shl bx, 5   ;в 'bx' medium * [len_cell] -> в 'si+bx' начало arr_keys[medium]
    add si, bx                  

binary_search:         
    pusha   ;сохраняем регистры, что они не изменились, после сравнения строк           
    call compare_string ;сравниваем строки
    popa            ;ключ - вторая. ключ из массива - первая
    cmp [flag], 1   ;0 - одинаковы; 1 - первая больше; 2 - вторая больше
        je dec_right
        jg inc_left
        jl print 
        
dec_right:
    mov cx, bx
    shr cx, 5
    dec cx  ;right = mediun - 1
    jmp pre_binary_search2
    
inc_left:
    mov ax, bx
    shr ax, 5
    inc ax  ;left = mediun + 1
    jmp pre_binary_search2
    
print:
    mov ah, 09h
    mov dx, ent
    int 21h ;выводим перевод строки
    
    mov di, arr_values
    add di, bx  ;пихаем в 'di' начало arr_values и смещаем до нужной ячейки 
    
    mov ah, 09h	 ;Номер функции DOS.
	mov dx, di    ;В DX адрес строки.
    int 21h ;выводим значение       
        
    jmp pre_read_key2   ;и идем считывать следующий ключ (если он конечно будет)    
;-----------------------------------------------------------------------    
error_exit: ;завершение программы, с ошибкой 
    mov ah, 09h     ;Функция DOS 09h - вывод строки
    mov dx, error   ;в DX адрес строки
    int 21h	    ;обращение к функции DOS
    jmp exit_programm   
      
no_error_exit_programm:	     ;завершение программы
    mov ah, 09h     ;Функция DOS 09h - вывод строки
    mov dx, press   ;в DX адрес строки
    int 21h	    ;обращение к функции DOS
 
exit_programm: 
    mov ah, 08h 	;функция DOS 08h - ввод символа без эха
    int 21h	    ;обращение к функции DOS
 
    mov ax, 4C00h	
    int 21h	    ;завершение программы   
;---процедуры-------------------------------------------
sort:
    push ax ;сохраняем left
    push cx ;сохраняем right
    cmp ax, cx
        jge sort_exit ;если left >= right то exit
    mov bx, ax
    add bx, cx
    shr bx, 1   ;bx - medium
    mov dx, bx  ;swap bx - cx
    mov bx, cx  ;bx - right
    mov cx, dx  ;cx - medium
    push bx ;сохраняем right
    call sort
    pop bx  ;вытаскиваем
    mov dx, bx  ;swap bx - cx
    mov bx, cx  ;bx - medium
    mov cx, dx  ;cx - right
    inc bx  ;bx = medium + 1
    mov dx, ax  ;swap bx, ax
    mov ax, bx  ;ax = medium + 1
    mov bx, dx  ;bx - left
    push bx ;сохраняем left              
    call sort
    pop bx  ;вытаскиваем
    mov dx, ax  ;swap bx, ax
    mov ax, bx  ;ax = left
    mov bx, dx  ;bx = medium + 1
    dec bx ;bx = medium
    mov [left], ax
    mov [medium], bx
    mov [right], cx
    
merge:  ;ax - i; bx - j; cx - k   
    inc bx  ;bx = m + 1
    mov cx, ax  ;k = left, для прощего копирования во временный массив
    begin_while:
    cmp ax, [medium]
        jg end_while    ;если i > m выходим из цикла
    cmp bx, [right]
        jg end_while    ;и если j > r выходим из цикла       
        call init_compare_string    ;записываем в 'si' и 'di' начала сравниваемых ключей
        pusha
        call compare_string ;arr_keys[i] ? arr_keys[j]
        popa
        cmp [flag], 2   
            je second_greater   ;arr_keys[right] больше
        mov dx, bx  ;сохраняем в 'dx' ячейку, с которой свапаем
        call copy_str   ;arr_tmp_keys[k] = arr_keys[j] | arr_tmp_values[k] = arr_values[j]
        inc bx  ;увеличиваем j
        jmp pre_end_while
        jmp begin_while                
        second_greater:
            mov dx, ax
            call copy_str   ;arr_tmp_keys[k] = arr_keys[i] | arr_tmp_values[k] = arr_values[i]
            inc ax  ;увеличиваем i
        pre_end_while:
            inc cx  ;увеличиваем k
            jmp begin_while                                                         
    end_while:
        cmp ax, [medium]
            jg else_if  ;если i > medium -> в левой части больше нет элементов
        mov bx, ax  ;иначе присваем j значение i
        start_for_i:
            cmp bx, [medium]
                jg exit_else_if ;пока j <= medium
            mov dx, bx
            call copy_str   ;копируем оставшиеся строки в массив tmp
            inc bx
            inc cx       
            jmp start_for_i         
        else_if:                
        mov ax, bx
        start_for_j:    ;делаем аналогично, но для правой части
            cmp ax, [right]
                jg exit_else_if
            mov dx, ax
            call copy_str
            inc ax
            inc cx
            jmp start_for_j                
        exit_else_if:
    call copy_back  ;копируем отсортиванные ключи и значения обратно
sort_exit: 
    pop cx  ;вытаскиваем right
    pop ax  ;вытаскиваем left
    ret
    
copy_back:  
    mov ax, [left]  ;записываем в 'ax' начало
    mov cx, [right] ;а в 'cx' конец
    sub cx, ax
    inc cx  ;while cx != 0 do for
    shl ax, 5   ;узнаем необходимое смещение
    push bx ;сохраняем bx, так как используем bl
    copy_back_for:
        mov si, arr_keys
        mov di, arr_tmp_keys
        add si, ax
        add di, ax
    begin_copy_back_key:
        mov bl, [di]
        mov [si], bl
        cmp byte[di], '$'
            je end_copy_back_key    ;копируем ключ пока не конец строки
        inc di
        inc si
        jmp begin_copy_back_key
    end_copy_back_key:
        mov si, arr_values
        mov di, arr_tmp_values
        add si, ax
        add di, ax                
    begin_copy_back_value:
        mov bl, [di]
        mov [si], bl
        cmp byte[di], '$'   ;аналогично, пока не конец
            je end_copy_back_value
        inc di
        inc si
        jmp begin_copy_back_value
    end_copy_back_value:
    add ax, [len_cell]  ;переходим к следующей ячейки
    loop copy_back_for  ;цикл, который выполнится (right - left) + 1 раз
    pop bx        
    ret
        
copy_str:   ;аналогично предыдущей процедуре    
    push cx 
    shl dx, 5
    mov di, arr_keys
    add di, dx
    shl cx, 5    
    mov si, arr_tmp_keys
    add si, cx
    push bx
    begin_copy_key:
        mov bl, [di]
        mov [si], bl
        cmp byte[di], '$'
            je end_copy_key
        inc di
        inc si
        jmp begin_copy_key
    end_copy_key:
        mov di, arr_values
        add di, dx
        mov si, arr_tmp_values
        add si, cx                
    begin_copy_value:
        mov bl, [di]
        mov [si], bl 
        cmp byte[di], '$'
            je end_copy_value
        inc di
        inc si
        jmp begin_copy_value
    end_copy_value:
    pop bx            
    pop cx
    ret
    
init_compare_string:    
    mov di, arr_keys    ;записываем в 'di' адрес начала arr_keys
    mov si, di  ;копируем адрес в 'si'
    shl ax, 5   ;вычисляем смещение
    add si, ax  ;смещаем 'si' на i-тую ячейку
    shr ax, 5   ;возвращаем ax значение i 
    shl bx, 5   ;аналогично
    add di, bx  ;смещаем 'di' на j-тую ячейку
    shr bx, 5
    ret    
            
compare_string: ;сраниваем строки. в 'si' и 'di' уже необходимы адреса
    mov dl, [si]    ;записываем в dl символ по адресу, записанному в 'si'    
    mov dh, [di]    ;аналогично
	cmp dl, '$'    ;если 'dl' == '$', возможно, что первая строка меньше если (abc$ и abcd$) либо эквивалентна (abc$ и abc$)
		je maybe_first_less
	cmp dh, '$'    ;если 'dh' == '$', то в любом случаем вторая меньше, т.к. проверка на первую была раньше
		je second_less
    cmp dl, dh  ;если не конец строки, то сраниваем символы 
		jl first_less ;'dl' < 'dh'
        jg second_less  ;'dl' > 'dh'                              
    inc si  ;если символы одинаковые
    inc di  ;то идем к следующим       
    jmp compare_string
        
maybe_first_less:
    cmp dh, '$' ;достаточно проверить, кончилась ли вторая строка
        je first_equal_second
first_less:        
    mov [flag], 2   ;вторая больше
    ret        

second_less:    
    mov [flag], 1   ; первая больше
    ret
    
first_equal_second: ;пригодится только при дихотомии, т.к. считается, что нет одинаковых ключей и значений
    mov [flag], 0   ;одинаковые
    ret  
;---конец--------------------------------------
