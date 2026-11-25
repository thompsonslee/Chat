console.log("hello!!");

const link = "http://localhost:3000"
//need to check cookies
const body = document.body;

interface html{
    login: string,
}

async function main(){
    const login: string = await getLoginHTML()
    document.body.innerHTML = login;
}

async function getLoginHTML():Promise<string> {
    const res = await fetch(link + "login");

    if(res.ok){
        return res.text();
    }
    throw new Error("failed to fetch loginHTML");
}
//if no cookie, fetch login screen html

