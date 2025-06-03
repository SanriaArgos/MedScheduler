import Link from "next/link";

export const Header = () => {
    return <header className="w-full h-14 xl:h-20 z-[100] fixed top-0 left-0 bg-main">
        <div className="w-full h-full max-w-screen-2xl mx-auto flex items-center justify-between px-2.5">
            <div className="bg-[red] w-[140px] h-full">
                Логотип<br/>(желательно горизонтальный)
            </div>
            <div className="w-fit h-full flex gap-4 xl:gap-8 font-medium text-[16px] xl:text-[24px] items-center leading-none">
                <Link href={"/"}>Главная</Link>
                <Link href={"/#about"}>О нас</Link>
                <Link href={"/login"}>Войти</Link>
            </div>
        </div>
    </header>
}