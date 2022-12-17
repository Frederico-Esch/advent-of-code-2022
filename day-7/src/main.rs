use std::{fs, io::{BufReader, BufRead}, vec};

#[derive(Debug)]
enum Command {
    LS,
    CD(String)
}


#[derive(Debug)]
struct Dir {
    name: String,
    items: Vec<Storage>,
}


#[derive(Debug)]
struct File {
    _size: u64
}


#[derive(Debug)]
enum Storage {
    DIR(Dir),
    FILE(File)
}

trait Sizable {
    fn size(&self) -> u64;
}

impl Sizable for File {
    fn size(&self) -> u64 {
        self._size
    }
}

impl Storage {

    fn find_minimum_free(&self, needed_to_free: u64, result: &mut u64) {
        if let Storage::DIR(dir) = self {
            let size = dir.size();
            if size >= needed_to_free && size < *result{
                *result = size;
            }
            for s in dir.items.iter() {
                s.find_minimum_free(needed_to_free, result);
            }
        }
    }

    fn sizes(&self, tamanhos:&mut Vec<(String, u64)>){
        if let Storage::DIR(dir) = self {
            let size = dir.size();
            println!("{} -> {}", dir.name, size);
            if size <= 100000{
                tamanhos.push((dir.name.clone(), size));
            }
            for s in dir.items.iter() {
                s.sizes(tamanhos);
            }
        }
    }

    fn into(&self) -> &dyn Sizable {
        match self {
            Storage::DIR(dir) => dir,
            Storage::FILE(file) => file
        }
    }

    fn add_dir(&mut self, dir: String, dirs: &[String]){
        println!("LEN {:?}", dirs);
        if dirs.len() == 0 {
            if let Storage::DIR(d) = self {
                d.items.push(Storage::DIR(Dir { name: dir, items: vec![] }));
            }
        }else {
            if let Storage::DIR(d) = self {
                let mut found = false;
                for item in d.items.iter_mut() {
                    if let Storage::DIR(subdir) = item {
                        if subdir.name == dirs[0] {
                            found = true;
                        }
                    }
                    if found {
                        item.add_dir(dir, &dirs[1..]);
                        break;
                    }
                }
            }
        }
    }

    fn add_file(&mut self, file: File, dirs: &[String]){
        if dirs.len() == 0 {
            if let Storage::DIR(d) = self {
                d.items.push(Storage::FILE(file));
            }
        }else {
            if let Storage::DIR(d) = self {
                let mut found = false;
                for item in d.items.iter_mut() {
                    if let Storage::DIR(subdir) = item {
                        if subdir.name == dirs[0] {
                            found = true;
                        }
                    }
                    if found {
                        item.add_file(file, &dirs[1..]);
                        break;
                    }
                }
            }
        }
    }
}

impl File {
    fn new(line: Vec<&str>) -> Self {
        Self { _size: line[0].parse().unwrap() }
    }
}


impl Sizable for Dir {
    fn size(&self) -> u64 {
        let mut result = 0;

        for item in self.items.iter() {
            result = result + item.into().size();
        }

        result
    }
}

impl Command {
    fn new(line:&str) -> Self{
        let line : Vec<&str> = line.split(' ').collect();
        if line.len() == 2 {
            Command::LS
        }else {
            Command::CD(line[2].to_string())
        }
    }
}

fn main() {
    let file = fs::File::open("input").unwrap();
    let buf = BufReader::new(file);
    let mut storage = Storage::DIR(Dir { name: "/".to_string(),  items : vec![]});
    let mut dirs = vec![];

    for line in buf.lines(){
        if let Ok(line) = line {
            if line.len() < 2 {
                continue;
            }

            println!("{}", line);

            if line.chars().next() == Some('$') {
                let cmd = Command::new(&line);
                if let Command::CD(dir) = cmd {
                    if dir == ".." {
                        dirs.pop();
                    } else if dir != "/" {
                        dirs.push(dir.clone());
                    }
                }else {
                    println!("LS at {}", dirs.join("/"));
                }
            } else {
                let line: Vec<&str> = line.split(" ").collect();
                if line[0] == "dir" {
                    storage.add_dir(line[1].to_string(), &dirs);
                }else {
                    let file = File::new(line);
                    storage.add_file(file, &dirs);
                }
            }
        }
    }

    let mut tamanhos = vec![];
    storage.sizes(&mut tamanhos);
    let sum = tamanhos.iter().fold(0u64, |acc, (_, s)| s + acc);
    let mut total_used = 0;
    if let Storage::DIR(root) = &storage {
        total_used = root.size();
    }

    let necessary_to_free = 30000000 - (70000000 - total_used);

    let mut result = u64::max_value();
    storage.find_minimum_free(necessary_to_free, &mut result);

    println!("{}", sum);
    println!("{}", result);
}
