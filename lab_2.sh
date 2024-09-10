#!/bin/sh
end_read_1=0
end_read_2=0
count_signals=0
delay_signals=0

handle() {
	count_signals=`expr $count_signals + 1`
	if [ $end_read_1 -eq 0 ]
	then
		delay_signals=`expr $delay_signals + 1`
		echo "Повторите ввод имени файла: "
		return
	fi

	if [ $end_read_2 -eq 0 ]
	then
		delay_signals=`expr $delay_signals + 1`
		echo "Повторите ввод имени файла: "
		return
	fi

	if [ $count_signals -eq 1 ]
	then
		echo "Количество строк, слов и символов в первом файле: "
		wc $file_1
	fi
	
	if [ $count_signals -eq 2 ] 
	then
		echo "Количество строк, слов и символов во втором файле: "
		wc $file_2
	fi	
}

trap handle 2

while :
do
	echo "Введите имя первого файла: "
	read file_1
	$end_read_1=1
	if [ -r $file_1 ]
	then
		break
	else
		echo "Файл не найден"
	fi	
done

while :
do
	echo "Введите имя второго файла: "
	read file_2
	$end_read_2=1
	if [ -r $file_2 ]
	then
		break
	else
		echo "Файл не найден"
	fi
done

end_read=1

if [ $delay_signals -eq 1 ]
then
        echo "Количество строк, слов и символов в первом файле: "
        wc $file_1
fi

if [ $delay_signals -ge 2 ]
then
        echo "Количество строк, слов и символов в первом файле: "
        wc $file_1
        echo "Количество строк, слов и символов во втором файле: "
        wc $file_2
fi

echo "Количество строк, слов и символов в двух файлах: "
cat $file_1 $file_2 | wc