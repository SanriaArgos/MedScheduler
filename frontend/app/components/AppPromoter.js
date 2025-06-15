import Image from 'next/image';
export const AppPromoter = () => {
    return <div
        className="mt-10 mb-10 xl:mt-14 rounded-[30px] flex flex-col xl:flex-row xl:justify-between xl:gap-x-32 overflow-hidden w-full h-fit bg-linear-65 from-main to-main2 min-h-40 px-4 xl:px-10 py-4 xl:py-10">
        <div className="xl:ml-4">
            <h3 className="font-bold text-[24px] xl:text-[32px] text-[white]">Заботимся о вашем времени</h3>
            <p className="mt-4 font-medium text-[16px] xl:text-[18px] text-[white]">
                Наше настольное приложение делает запись к врачу еще удобнее:
            </p>            <ul className="mt-2 list-disc list-inside text-white text-[16px] xl:text-[18px]">
                <li>Быстрый поиск специалистов</li>
                <li>Управление записями в пару кликов</li>
                <li>Работает без интернета</li>
                <li>Быстрее веб-версии</li>
                <li>И многое другое</li>
            </ul>
            
            <div className="mt-6 flex flex-col sm:flex-row gap-3">
                <a 
                    href="https://github.com/SanriaArgos/MedScheduler/releases/tag/Windows"
                    target="_blank"
                    rel="noopener noreferrer"
                    className="inline-flex items-center justify-center px-6 py-3 bg-white hover:bg-gray-100 text-main font-semibold rounded-lg transition-all duration-200 shadow-lg hover:shadow-xl text-[16px] xl:text-[18px]"
                >
                    <svg className="w-5 h-5 mr-2" fill="currentColor" viewBox="0 0 20 20">
                        <path fillRule="evenodd" d="M3 17a1 1 0 011-1h12a1 1 0 110 2H4a1 1 0 01-1-1zm3.293-7.707a1 1 0 011.414 0L9 10.586V3a1 1 0 112 0v7.586l1.293-1.293a1 1 0 111.414 1.414l-3 3a1 1 0 01-1.414 0l-3-3a1 1 0 010-1.414z" clipRule="evenodd" />
                    </svg>
                    Скачать для Windows
                </a>                <div className="flex items-center text-white/90 text-[14px] xl:text-[16px]">
                    <svg className="w-4 h-4 mr-1" fill="currentColor" viewBox="0 0 20 20">
                        <path fillRule="evenodd" d="M10 18a8 8 0 100-16 8 8 0 000 16zm3.707-9.293a1 1 0 00-1.414-1.414L9 10.586 7.707 9.293a1 1 0 00-1.414 1.414l2 2a1 1 0 001.414 0l4-4z" clipRule="evenodd" />
                    </svg>
                    Бесплатно
                </div>
            </div>
        </div>
        <div className="mt-4 xl:mt-0 flex-shrink-0">
            <div className="xl:h-[500px] w-full xl:w-[800px] max-w-full aspect-[16/9] flex relative text-[24px] rounded-[20px] bg-[white]">
                <Image src="/release1506.png" alt="MedScheduler App Screenshot"
                       fill className="object-contain p-4" sizes="100vw"/>
            </div>
        </div>
    </div>
}

