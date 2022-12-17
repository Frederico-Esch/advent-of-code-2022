use std::{fs, io::Read, mem::swap};

struct Group {
    items : [Option<char>;14],
}

impl Group {
    pub fn new() -> Self { Self { items : [None;14] } }
    #[allow(unused)]
    fn has_all_values(&self) -> bool {
        self.items.iter().map(|&o| {
            o != None
        }).fold(true, |acc, b| acc | b)
    }

    pub fn different(&self) -> bool {
        let mut number = self.items
        .iter()
        .filter_map(|&o| if let Some(c) = o {
                Some((c as u8) - ('a' as u8))
            } else {
                None
            })
        .fold(0u64, |acc, c| acc | (1u64 << c));

        let mut bits = 0usize;
        while number != 0 {
            if number & 1 == 1 {
                bits += 1;
            }
            number >>= 1;
        }
        bits == self.items.len()
    }

    pub fn add(&mut self, c : char) {
        let mut last = self.items[0];
        self.items[0] = Some(c);

        for i in 1..self.items.len() {
            if last == None {
                break;
            }else {
                swap(&mut last, &mut self.items[i]);
            }
        }
    }
}


fn main() {
    let mut file = fs::File::open("input").unwrap();
    let mut buf = String::new();
    let _ = file.read_to_string(&mut buf);
    let mut group = Group::new();

    let mut count = 0u64;
    for c in buf.chars() {
        count += 1;
        group.add(c);
        if group.different() {
            break;
        }
    }
    println!("{}", count);
}
