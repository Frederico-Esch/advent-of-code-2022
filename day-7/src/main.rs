
#[allow(unused)]
enum Command {
    LS,
    CD(String)
}

#[allow(unused)]
struct Dir {
    items: Vec<Storage>,
}

#[allow(unused)]
struct File {
    _size: u64
}

#[allow(unused)]
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

#[allow(unused)]
impl Sizable for Dir {
    fn size(&self) -> u64 {
        let mut result = 0;

        for item in self.items.iter() {
            result = result + match item {
                Storage::DIR(dir) => dir.size(),
                Storage::FILE(file) => file.size()
            };
        }

        result
    }
}

fn main() {
    println!("Hello, world!");
}
