<?php 

/*
 * Сборка происходит таким образом:
 * 
 * main.cc        @ g++ `sdl-config --cflags --libs` %so -lSDL -lglut -lGL -lGLU -Wall -o main
 *   objects.cc   # g++ -c %s -o %s.o
 *   render.cc
 */

if (file_exists("makefile")) {
    
    $struct = [];
    $config = array_map('rtrim', file("makefile"));
    
    if (file_exists('.history.log')) {
        $histlog = json_decode(file_get_contents('.history.log'), 1);
    } else {
        file_put_contents('.history.log', json_encode($histlog = []));
    }

    $ident_id  = 0;
    $identleft = 0;
    $parent_id = 0;
    $cursor_id = 0;
    $cursor_id_pivot = 0;
    $ident_struct = [];
    $ident_hist = [];
    $uncmd = [];
    
    foreach ($config as $row) {                

        $row = rtrim(preg_replace('~#.*$~', '', $row));
        if (trim($row) === '') {
            continue;
        }
        
        $cursor_id++;
        
        $command  = '';
        $filename = trim($row);

        if (preg_match('~^(.*?)@(.+)$~', $row, $c)) {
            $filename = trim($c[1]);
            $command  = trim($c[2]);            
        }
        
        // Количество пробельных символов слева    
        $spaceleft = strlen($row) - strlen(ltrim($row));
        
        // Новый отступ направо
        if ($identleft < $spaceleft) {
                        
            $ident_id++;            
            $parent_id = $cursor_id_pivot;  
            
            // Сохранить старый parent_id для данной структуры
            $ident_struct[ $ident_id ] = $parent_id;
            $ident_hist  [ $ident_id ] = $spaceleft;
                      
        } 
        // Отступ налево: вернуть прежний parent_id
        else if ($identleft > $spaceleft) {
            
            $ident_id = 0;
            
            // Найти отступ 
            foreach ($ident_hist as $ID => $LN) {        
                if ($LN == $spaceleft) {
                    $ident_id = $ID;
                    break;
                }            
            }
            
            $parent_id = $ident_id ? $ident_struct[ $ident_id ] : 0;
        }
        
        // Монолитность
        $identleft = $spaceleft;
    
        $struct[ $cursor_id ] = [
            'filename' => $filename,
            'command'  => $command,
            'parent_id' => $parent_id,
        ];
        
        // Для случая, чтобы стать parent
        $cursor_id_pivot = $cursor_id;
    }   

    $rowtouch = [];
    
    // Определить, какие файлы надо пересобрать заново
    foreach ($struct as $id => $row) {
        
        $fn = $row['filename'];
        if (file_exists($fn) && $fn) {
            
            if (isset($histlog[ $fn ]['mtime'])) {
                $mtime = $histlog[ $fn ]['mtime'];
            } else {
                $mtime = 0;
            }
            
            $ftime = filemtime($fn);
            
            // Файлы для пересборки
            if ($mtime != $ftime) {                
                $rowtouch[] = $id;
            }
            
            $histlog[ $fn ]['mtime'] = $ftime;
            
        } elseif ($fn) {
            
            echo "File not found: $fn\n";
            exit(1);

        } else {
            
            if ($row['command']) {                
                $uncmd[] = $row['command'];
            }
        }
    }
    
    foreach ($rowtouch as $id) {
        
        do {
            
            $row = $struct[ $id ];
            $filename = $row['filename'];
            $id = $row['parent_id'];
            
            // Не обрабатывать тут такие
            if (empty($filename)) {
                break;
            }
                        
            if ($row['command']) { // Особая команда для выполнения
                $cmd = $row['command'];                
            }             
            else { // Обычный сборщик
                
                $cmd = "";
                
                // GCC
                if (preg_match('~^(.+)\.(cc|cpp)$~i', $filename, $c)) {
                    $cmd = "g++ -c '$filename' -o '".$c[1].".o'";
                }
                
                if (empty($cmd)) {
                    echo ("Command not defined for `$filename`");
                    exit(1);
                }                
            }
                                    
            $cmd = str_replace('%s', $filename, $cmd);
            echo ">> $cmd\n";

            exec($cmd, $output, $errcode);            
            if ($errcode > 0) {
                echo "Exec($errcode)\n";
                exit($errcode);    
            }
            
        } while ($id);        
    }
    
    foreach ($uncmd as $cmd) {
        
        exec($cmd, $output, $errcode);            
        if ($errcode > 0) {
            echo ("Exec($errcode): $cmd\n");                
            exit(1);
        }        
    }

    file_put_contents('.history.log', json_encode($histlog));
    
    echo "OK\n";
    
} else {
    
    echo "File `makefile` not found\n";
}
