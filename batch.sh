echo  "Execution of outliers"

DB_NAME=agrodata
DB_TRAIN=/home/aocsa/datasets/$DB_NAME/$DB_NAME.TRAIN
DB_DIM=20
echo $DB_NAME
./outliers $DB_DIM 10 1 5 BF $DB_TRAIN
./outliers $DB_DIM 10 1 5 LSH $DB_TRAIN

DB_NAME=audio
DB_TRAIN=/home/aocsa/datasets/$DB_NAME/$DB_NAME.TRAIN
DB_DIM=192
echo $DB_NAME
./outliers $DB_DIM 10 1 5 BF $DB_TRAIN
./outliers $DB_DIM 10 1 5 LSH $DB_TRAIN

DB_NAME=random-walk
DB_TRAIN=/home/aocsa/datasets/$DB_NAME/$DB_NAME.TRAIN
DB_DIM=64
echo $DB_NAME
./outliers $DB_DIM 10 1 5 BF $DB_TRAIN
./outliers $DB_DIM 10 1 5 LSH $DB_TRAIN

DB_NAME=ucayalli
DB_TRAIN=/home/aocsa/datasets/$DB_NAME/$DB_NAME.TRAIN
DB_DIM=34
echo $DB_NAME
./outliers $DB_DIM 10 1 5 BF $DB_TRAIN
./outliers $DB_DIM 10 1 5 LSH $DB_TRAIN

echo  "Execution of motifs"

DB_NAME=agrodata
DB_TRAIN=/home/aocsa/datasets/$DB_NAME/$DB_NAME.TRAIN
DB_DIM=20
echo $DB_NAME
./motifs serial $DB_TRAIN
./motifs parallel $DB_TRAIN

DB_NAME=audio
DB_TRAIN=/home/aocsa/datasets/$DB_NAME/$DB_NAME.TRAIN
DB_DIM=192
echo $DB_NAME
./motifs serial $DB_TRAIN
./motifs parallel $DB_TRAIN

DB_NAME=random-walk
DB_TRAIN=/home/aocsa/datasets/$DB_NAME/$DB_NAME.TRAIN
DB_DIM=64
echo $DB_NAME
./motifs serial $DB_TRAIN
./motifs parallel $DB_TRAIN

DB_NAME=ucayalli
DB_TRAIN=/home/aocsa/datasets/$DB_NAME/$DB_NAME.TRAIN
DB_DIM=34
echo $DB_NAME
./motifs serial $DB_TRAIN
./motifs parallel $DB_TRAIN
