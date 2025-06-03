import Link from "next/link";

export const Footer = () => {
    return <footer className="w-full flex flex-col bg-[black] text-[white] py-2.5 xl:py-10">
        <div className="w-full max-w-screen-2xl mx-auto flex items-center justify-between px-2.5 ">
            <div className="flex flex-col gap-y-4 xl:gap-y-8 font-medium text-[16px] xl:text-[24px] leading-none">
                <Link href={"/"}>Главная</Link>
                <Link href={"/#about"}>О нас</Link>
                <Link href={"/login"}>Войти</Link>
            </div>
        </div>

        <div className="w-full max-w-screen-2xl mx-auto flex font-medium text-[12px] xl:text-[20px] items-center justify-between px-2.5 mt-4 xl:mt-10">
            © 2025, MedScheduler
        </div>
    </footer>
}