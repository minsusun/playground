use std::any;
use std::mem;

fn _type<T>(t: &T) {
    println!("Type: {}", any::type_name::<T>());
    println!("Size of Var Type: {}Bytes", mem::size_of::<T>());
    println!("Size of Var: {}Bytes", mem::size_of_val(&t));
}

fn main() {
    let a = 10;
    _type(&a);
}