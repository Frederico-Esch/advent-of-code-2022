use std::{fs, io::{BufReader, BufRead}};

struct SpecialSet {
    value: u64
}

impl SpecialSet {
    pub fn new() -> Self{
        SpecialSet { value: 0 }
    }

    pub fn clear(&mut self){
        self.value = 0;
    }

    fn u8_to_index(val: u8) -> u8{
        if val >= 97 {
            val-97
        }else {
            val - 65 + 26
        }
    }

    pub fn u8_to_score(val:u8) -> u8 {
        if val >= 97 {
            val-96
        }else {
            val - 65 + 27
        }
    }

    pub fn contains(&self, val: &char) -> bool{
        let val = *val as u8;
        self.value & (1 << Self::u8_to_index(val)) > 0
    }

    pub fn push(&mut self, val: &char){
        let val = *val as u8;
        let val = Self::u8_to_index(val);
        self.value |= 1 << val;
    }
}

#[warn(unused)]
fn primeira(){
    let file = fs::File::open("../input").unwrap();
    let buf = BufReader::new(file);
    let mut val = SpecialSet::new();
    let mut results = vec![];

    for line in buf.lines() {
        let line = line.unwrap();

        val.clear();
        for c in line.chars().filter(|&c| c != '\n').take(line.len()/2) {
            val.push(&c);
        }

        for c in line.chars().filter(|&c| c != '\n').skip(line.len()/2) {
            if val.contains(&c) {
                print!("Char encontrado {}", c);
                let c = c as u8;
                let c = SpecialSet::u8_to_score(c);
                println!(" Valor {}", c);
                results.push(c);
                break;
            }
        }

    }

    let result = results.iter().fold(0u64, |acc, &x| acc+(x as u64));
    println!("SUM: {}", result);
}

fn intersect_three(primeiro: &SpecialSet, segundo: &SpecialSet, terceiro: &SpecialSet) -> u64{
    let value = primeiro.value & segundo.value & terceiro.value;
    return value;
}

fn index_to_score(value: &u64) -> u64{
    let mut val = *value;
    let mut index = 0;
    while val != 0 {
        val >>= 1;
        index += 1;
    }
    index
}

fn segundo(){
    let file = fs::File::open("input").unwrap();
    let buf = BufReader::new(file);
    let mut val = [ SpecialSet::new(), SpecialSet::new(), SpecialSet::new() ];
    let mut results :Vec<u64> = vec![];
    let mut group = 0usize;

    for line in buf.lines() {
        let line = line.unwrap();

        for c in line.chars().filter(|&c| c != '\n') {
            val[group].push(&c);
        }

        group += 1;
        if group == 3 {
            group = 0;
            //get the value intersected from vals
            let result = intersect_three(&val[0], &val[1], &val[2]);
            println!("RESULT:\n{}\n{}\n{}", val[0].value, val[1].value, val[2].value);
            results.push(index_to_score(&result));
            val[0].clear();
            val[1].clear();
            val[2].clear();
        }

    }

    for res in results.iter() {
        println!("RES: {}", res);
    }

    println!("SUM: {}", results.iter().fold(0u64, |acc, &c| acc+c));
}

fn main() {
    segundo();
}

