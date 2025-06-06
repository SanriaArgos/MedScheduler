import Link from "next/link";
import Image from "next/image"; // Добавлено для логотипа

export const Header = () => {
    return <header className="w-full h-14 xl:h-20 z-[100] fixed top-0 left-0 bg-main">
        <div className="w-full h-full max-w-screen-2xl mx-auto flex items-center justify-between px-2.5">
            <Link href="/" className="flex items-center h-full">
                {/* Замените на ваш реальный логотип */}
                <Image src="/icon-512.png" alt="MedScheduler Logo" width={48} height={48} className="h-10 w-10 xl:h-12 xl:w-12"/>
                <span className="ml-2 text-xl xl:text-2xl font-bold text-white">MedScheduler</span>
            </Link>
            <nav className="w-fit h-full flex gap-3 xl:gap-6 font-medium text-[15px] xl:text-[20px] items-center leading-none text-white">
                <Link href={"/"} className="hover:text-main2 transition-colors">Главная</Link>
                <Link href={"/#about"} className="hover:text-main2 transition-colors">О нас</Link>
                <Link href={"/api-documentation"} className="hover:text-main2 transition-colors">API</Link>
                <Link href={"/login"} className="bg-main2 hover:bg-opacity-80 transition-all text-white px-4 py-2 rounded-md">Войти</Link>
            </nav>
        </div>
    </header>
}

